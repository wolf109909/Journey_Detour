#include "igig.h"
#include "kiero.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "spdlog/spdlog.h"
#include "../game.h"
#include <thread>
typedef HRESULT(__stdcall* fpPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* fpWndProc)(HWND, UINT, WPARAM, LPARAM);

ImGuiManager::ImGuiManager() {}

ImGuiManager& ImGuiManager::instance()
{
	static ImGuiManager IGIG;
	return IGIG;
}

fpPresent originalPresent;
fpWndProc originalWndProc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT __stdcall hookedWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
	/*if (ImGui::GetIO().WantCaptureMouse)
	{
		return true;
	}*/
	return CallWindowProc(originalWndProc, hWnd, uMsg, wParam, lParam);
}

static HRESULT __stdcall hookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	//return originalPresent(pSwapChain, SyncInterval, Flags);
	auto& igig = ImGuiManager::instance();
	if (!igig.init)
	{
		if (igig.pDevice.try_capture(pSwapChain, &IDXGISwapChain::GetDevice))
		{
			igig.pDevice->GetImmediateContext(igig.pContext.put());

			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			igig.window = sd.OutputWindow;

			
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

			ImGui_ImplWin32_Init(igig.window);
			ImGui_ImplDX11_Init(igig.pDevice.get(), igig.pContext.get());
			

			io.Fonts->AddFontDefault();
			ImFontConfig font_config_chs;
			strcpy_s(font_config_chs.Name, "MicrosoftYaHei");
			font_config_chs.GlyphOffset = ImVec2(0.0, 1.0);
			font_config_chs.MergeMode = true;
			font_config_chs.PixelSnapH = true;
			font_config_chs.OversampleH = 2;
			font_config_chs.OversampleV = 1;
			io.Fonts->AddFontFromFileTTF("c:\\windows\\fonts\\msyh.ttc", 15.0f, &font_config_chs, io.Fonts->GetGlyphRangesChineseFull());

			ImGui_ImplDX11_CreateDeviceObjects();
			originalWndProc = (WNDPROC)SetWindowLongPtr(igig.window, GWLP_WNDPROC, (LONG_PTR)hookedWndProc);

			igig.init = true;
		}
		else
		{
			return originalPresent(pSwapChain, SyncInterval, Flags);
		}
	}
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (g_pGame->m_LocalDude != nullptr)
	{
		ImGui::Text(fmt::format("RemoteDude Name: {}", g_pGame->GetPartnerName()).c_str());

		ImGui::Checkbox("InfiniteScarf", &g_pGame->b_InfiniteScarf);

		ImGui::Text(std::to_string(g_pGame->m_LocalDude->XPos).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->YPos).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->ZPos).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->XAccel).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->YAccel).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->ZAccel).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->ScarfMax).c_str());
		ImGui::Text(std::to_string(g_pGame->m_LocalDude->ScarfCharge).c_str());
	}


	ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiKey_Tab, ImGuiKeyOwner_None);
	ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab, ImGuiKeyOwner_None);
	
	for (const auto& func : igig.drawFunctions)
	{
		func();
	}
	
	ImGui::EndFrame();
	ImGui::Render();

	

	
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	return originalPresent(pSwapChain, SyncInterval, Flags);
}

void ImGuiManager::startInitThread()
{
	std::thread initThread(
		[]()
		{
			while (true)
			{
				if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					continue;
				}
				if (kiero::bind(8, (void**)&originalPresent, hookedPresent) != kiero::Status::Success)
				{
					spdlog::error("[IGIG] Kiero bind failed");
					break;
				}
				else
				{
					spdlog::info("[IGIG] Kiero bind succeeded");
					break;
				}
				break;
			}
				
			
		});
	initThread.detach();
}

void ImGuiManager::addDrawFunction(std::function<void()>&& f)
{
	drawFunctions.push_back(std::move(f));
}
