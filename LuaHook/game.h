#pragma once

#ifndef __GAME_H__
#define __GAME_H__
#include "stdafx.h"
#include "luahooks.h"

// This namespace provides base address of things.
//using namespace Lua;
class Game 
{
	Lua::lua_State* LuaState;

	typedef void(__cdecl* _gameupdate)(__int64 a1, float a2);
	typedef int GetLevelUidType(Lua::lua_State*, char* LevelName);

	GetLevelUidType* GetLevelUid = (GetLevelUidType*)0x1400DA830;
	_gameupdate origUpdateFunction = NULL;

	bool isNetGUIEnabled = false;
	bool locateGame;
	bool autolobbybool1;
	bool autolobbybool2;
	bool isNetGUIEnabled;

	unsigned __int8 someinteger;

	uintptr_t BaseGame;
	uintptr_t BaseRender;
	uintptr_t BaseMatchmaker;

	void PreGameUpdate();
	void UpdateValues();
	void Update(__int64 game, float a2);
	char* CurrentLevelName();
	int CurrentLevelUid();
	void Initialize();
};

extern Game* g_Game;
#endif