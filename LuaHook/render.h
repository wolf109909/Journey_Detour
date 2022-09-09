#pragma once
#include "hookutils.h"
#include "lua.h"
#include "guiconsole.h"
#include "custommenu.h"
#include "game.h"


namespace Render 
{

	typedef void(__cdecl* _netgui)(__int64 a1, __int64 a2, float a3);
	_netgui origNetGuiFunction = NULL;

	typedef int(__cdecl* _addtext)(__int64 a1, const char* text, float x, float y, float size, int color);
	_addtext Addtext = NULL;


	int AddTextHook(__int64 a1, const char* text, float x, float y, float size, int color)
	{
		return Addtext(a1, text, x, y, size, color);
	}

	void CustomTextDoRender()
	{
		GUIConsole::Draw();
		MenuSystem::GUI::Draw();

	}

	void NetGuiHook(__int64 a1, __int64 a2, float a3)
	{
		// add custom text render barn here cuz the game will call it while queueing render is possible
		CustomTextDoRender();
		Addtext(Game::BaseRender, "Journey Detour v1.0.1", -1.75, -1.0, 0.05, 0xFFFFFFFF);
		origNetGuiFunction(a1, a2, a3);
	}

	void Initialize() 
	{
		HookEnabler hook;
		ENABLER_CREATEHOOK(
			hook, (LPVOID)0x14026EAA0, &AddTextHook, reinterpret_cast<LPVOID*>(&Addtext));
		ENABLER_CREATEHOOK(
			hook, (LPVOID)0x14010DA00, &NetGuiHook, reinterpret_cast<LPVOID*>(&origNetGuiFunction));
	}
}