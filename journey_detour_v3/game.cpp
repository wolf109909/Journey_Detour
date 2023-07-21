#include "framework.h"
#include <windows.h>
#include <psapi.h>
#include <set>
#include <map>
#include <filesystem>
#include <sstream>
#include "spdlog/spdlog.h"
#include "hooks.h"
#include "game.h"
#include "lua.h"
#include <stdint.h>
AUTOHOOK_INIT()



GameManager* g_pGame;

typedef char*(__cdecl* _game_GetPartnerName)(uintptr_t network);
_game_GetPartnerName game_GetPartnerName = MemoryAddress(0x140100400).As<_game_GetPartnerName>();


AUTOHOOK_ABSOLUTEADDR(AddDecoration, (LPVOID)0x14009F9D0,
	uintptr_t, __fastcall, (uintptr_t NOTdecoBarn, __int64 resources, char* name1, char* name2, vec_mat* a5))
{	
	spdlog::info("ptr:{},size:{}", g_pGame->m_Game->DecorationBarn, *(unsigned int*)(g_pGame->m_Game->DecorationBarn + 4407312));
	return AddDecoration(g_pGame->m_Game->DecorationBarn, resources, name1, name2, a5);
}

AUTOHOOK_ABSOLUTEADDR(Debug_print, (LPVOID)0x1402E3090,
	const char*, __fastcall, (const char* fmt, ...))
{
	if (!g_pGame->b_IsConsoleRedirectEnabled)
		return fmt;
	va_list args;
	char buffer[16384];
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);
	spdlog::info(buffer);
	return fmt;
}

AUTOHOOK_ABSOLUTEADDR(netgui_render, (LPVOID)0x14010DA00,
	void, __fastcall, (__int64 a1, __int64 a2, float a3))
{
	if (!g_pGame->b_Initialized)
		return netgui_render(a1, a2, a3);
	// Custom content rendering
	if (g_pGame->m_LocalDude != NULL) 
	{
		if (g_pGame->b_InfiniteScarf)
			g_pGame->m_LocalDude->ScarfCharge = g_pGame->m_LocalDude->ScarfMax;
		//g_pGame->m_LocalDude->XAccel = 1.0;
		//g_pGame->r_AddText(g_pGame->m_Game->Render, std::format("X:{} Y:{} Z:{}", floor(g_pGame->m_LocalDude->XPos * 10 + .5) / 10, floor(g_pGame->m_LocalDude->YPos * 10 + .5) / 10, floor(g_pGame->m_LocalDude->ZPos * 10 + .5) / 10).c_str(), -1.75, -1.0, 0.05, 0xFFFFFFFF);
		//dude codes here maybe
	}
	if (!g_pGame->m_vRenderCallbacks.empty()) 
	{
		for (RenderCallback* callback : g_pGame->m_vRenderCallbacks) {
			callback->Execute();
		}
	}
	g_pGame->r_AddText(g_pGame->m_Game->Render, "Journey Detour v3.0.1", -1.75, -1.0, 0.05, 0xFFFFFFFF);
	return netgui_render(a1, a2, a3);
}



AUTOHOOK_ABSOLUTEADDR(game_update, (LPVOID)0x1400DAD40,
    void, __fastcall, (Game* m_Game, float a2))
{
	// Update game offset values
	if (!g_pGame->b_Initialized) 
	{
		g_pGame->m_Game = m_Game;
		spdlog::info("[GameManager] Successfully obtained Game");
		g_pGame->r_AddText = MemoryAddress(0x14026EAA0).As<_addtext>();
		g_pGame->m_LocalDude = MemoryAddress(m_Game->PlayerBarn).As<LocalDude*>();
		spdlog::info("[GameManager] Successfully initialized rendering");
		g_pGame->g_GetConsole = MemoryAddress(0x140316DC0).As<_getconsole>();
		g_pGame->g_PrintLuaMem = MemoryAddress(0x1400F0B90).As<_printluamem>();
		g_pGame->g_AddDecoration = MemoryAddress(0x14009F9D0).As<_adddeco>();
		spdlog::info("[GameManager] Successfully detoured native functions");
		g_pGame->b_Initialized = true;
	}
	
	// Execute lua buffer
	if(g_pLua != nullptr)
		g_pLua->ExecuteBuffer();
	
	return game_update(m_Game, a2);
}

void GameManager::Initialize() 
{
	g_pGame = new GameManager;
	
	AUTOHOOK_DISPATCH()
}

void GameManager::AddRenderCallback(RenderCallback* callback)
{
	g_pGame->m_vRenderCallbacks.push_back(callback);
}

void GameManager::ToggleInfiniteScarf()
{
	this->b_InfiniteScarf = !this->b_InfiniteScarf;
}

char* GameManager::GetPartnerName()
{
	return game_GetPartnerName(this->m_Game->Network);
}

RenderCallback::RenderCallback(std::string name, RenderContext context)
{
	this->m_Name = name;
	this->m_Context = context;
}

void RenderCallback::Remove(std::string name) 
{
	for (RenderCallback* callback : g_pGame->m_vRenderCallbacks) {
		if (callback->m_Name == name)
			g_pGame->m_vRenderCallbacks.erase(std::remove(g_pGame->m_vRenderCallbacks.begin(), g_pGame->m_vRenderCallbacks.end(), callback), g_pGame->m_vRenderCallbacks.end());
	}
}
void RenderCallback::Execute() 
{
	this->m_Context();
};