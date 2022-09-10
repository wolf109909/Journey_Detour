//
// Created by scrci on 2022/9/10.
//
#pragma once
#ifndef CL1_GAME_H
#define CL1_GAME_H
#include "sharedlibrary.h"
#include "lua.h"

namespace Game
{
    using namespace Lua;
    extern uintptr_t game;
    extern uintptr_t render;
    extern uintptr_t matchmaker;
    //uintptr_t L;
    typedef int GetLevelUidType(Lua::lua_State*, char* LevelName);
    extern GetLevelUidType* GetLevelUid;
    extern bool locateGame;
    extern bool autolobbybool1;
    extern bool autolobbybool2;
    extern unsigned __int8 someinteger;

    void UpdateValues();

    char* CurrentLevelName();
    int CurrentLevelUid();
    void SetBaseAddress(uintptr_t addr);

};
#endif //CL1_GAME_H
