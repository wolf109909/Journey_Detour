#pragma once
#include "lua.h"
// This namespace provides base address of things.
namespace Game
{
	using namespace Lua;
	uintptr_t BaseGame;
	uintptr_t BaseRender;
	uintptr_t BaseMatchmaker;
	lua_State* LuaState;
	typedef void(__cdecl* _gameupdate)(__int64 a1, float a2);
	_gameupdate origUpdateFunction = NULL;

	typedef int GetLevelUidType(lua_State*, char* LevelName);
	GetLevelUidType* GetLevelUid = (GetLevelUidType*)0x1400DA830;

	bool isNetGUIEnabled = false;
	//bool locateGame = false;
	bool autolobbybool1;
	bool autolobbybool2;
	unsigned __int8 someinteger;
	

	void PreGameUpdate()
	{
		Lua::ExecuteBuffer();
	}


	void UpdateValues()
	{

		*LuaState = *(lua_State*)(BaseGame + 32);
		spdlog::info("LuaState Address:{}", *LuaState);
		BaseRender = *(int*)(BaseGame + 192);
		spdlog::info("BaseRender Address:{}", BaseRender);
		BaseMatchmaker = *(int*)(BaseGame + 384);
		spdlog::info("BaseMatchmaker Address:{}", BaseMatchmaker);
		//someinteger = *(unsigned __int8*)(matchmaker + 1536);
		//autolobbybool1 = *(bool*)(someinteger + 1057);
		//autolobbybool2 = *(bool*)(someinteger + 1058);

	}
	void Update(__int64 game, float a2)
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
	char* CurrentLevelName()
	{
		return (char*)(BaseGame + 1835760);
	}
	int CurrentLevelUid()
	{
		return GetLevelUid(lua_State_ptr, CurrentLevelName());
	}

	void Initialize()
	{
		HookEnabler hook;
		ENABLER_CREATEHOOK(
			hook, (LPVOID)0x1400DAD40, &Update, reinterpret_cast<LPVOID*>(&origUpdateFunction));

		
		

	}

};