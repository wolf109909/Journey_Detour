#pragma once
#include <functional>
#include <d3d11.h>
#include <winrt/base.h>

class ImGuiManager
{
  public:
	HWND window = NULL;
	winrt::com_ptr<ID3D11Device> pDevice;
	winrt::com_ptr<ID3D11DeviceContext> pContext;
	winrt::com_ptr<ID3D11RenderTargetView> mainRenderTargetView;
	bool init = false;
	std::vector<std::function<void()>> drawFunctions;

	static ImGuiManager& instance();

	void startInitThread();
	void addDrawFunction(std::function<void()>&& f);

  private:
	ImGuiManager();
};
