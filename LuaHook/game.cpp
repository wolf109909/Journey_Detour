#include "stdafx.h"
#include "game.h"
#include "luahooks.h"
#include "hookutils.h"

Game* g_Game = new(Game);



void Game::PreGameUpdate()
{
	Lua::ExecuteBuffer();
}


void Game::UpdateValues()
{

	*LuaState = *(Lua::lua_State*)(BaseGame + 32);
	spdlog::info("LuaState Address:{}", *LuaState);
	BaseRender = *(int*)(BaseGame + 192);
	spdlog::info("BaseRender Address:{}", BaseRender);
	BaseMatchmaker = *(int*)(BaseGame + 384);
	spdlog::info("BaseMatchmaker Address:{}", BaseMatchmaker);
	//someinteger = *(unsigned __int8*)(matchmaker + 1536);
	//autolobbybool1 = *(bool*)(someinteger + 1057);
	//autolobbybool2 = *(bool*)(someinteger + 1058);

}
void Game::Update(__int64 game, float a2)
{
	if (BaseGame == 0)
	{
		BaseGame = game;
		spdlog::info("Obtaining BaseGame...");
		UpdateValues();
		Sleep(1000);
	}
	else
	{
		PreGameUpdate();
	}
	origUpdateFunction(game, a2);
}
char* Game::CurrentLevelName()
{
	return (char*)(BaseGame + 1835760);
}
int Game::CurrentLevelUid()
{
	return GetLevelUid(LuaState, CurrentLevelName());
}

void Game::Initialize()
{
	HookEnabler hook;
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x1400DAD40, &Update, reinterpret_cast<LPVOID*>(&origUpdateFunction));
}
