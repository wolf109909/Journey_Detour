//
// Created by scrci on 2022/9/10.
//
#pragma once
#ifndef CL1_GAME_H
#define CL1_GAME_H

#include "sharedlibrary.h"
#include "lua.h"

namespace Game {
    typedef void(__cdecl *_tick)(__int64 a1, float a2);
//    typedef int GetLevelUidType(Lua::lua_State *, char *LevelName);
    //uintptr_t L;
//    extern uintptr_t        game;
    extern uintptr_t        render;
    extern uintptr_t        matchmaker;
    extern _tick            Tick;

//    extern GetLevelUidType  *GetLevelUid;
    extern bool             locateGame;
    extern bool             autolobbybool1;
    extern bool             autolobbybool2;
    extern unsigned __int8  someinteger;

    extern uintptr_t        GameRender;

    void UpdateValues();

    char *CurrentLevelName();

    int CurrentLevelUid();

    void SetBaseAddress(uintptr_t addr);

    void Initialize(HookEnabler hook);
};
#endif //CL1_GAME_H
