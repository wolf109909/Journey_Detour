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
    extern uintptr_t        BaseGame;
    extern uintptr_t        Render;
    extern uintptr_t        Matchmaker;
    extern uintptr_t        PlayerBarn;
    extern uintptr_t        LocalDude;
    extern uintptr_t        LuaState;
    extern _tick            Tick;

//    extern GetLevelUidType  *GetLevelUid;
    extern bool             locateGame;
    extern bool             autolobbybool1;
    extern bool             autolobbybool2;
    extern unsigned __int8  someinteger;


    void UpdateValues();

    char *CurrentLevelName();

    int CurrentLevelUid();

    void SetBaseAddress(uintptr_t addr);

    void Initialize(HookEnabler hook);
};
#endif //CL1_GAME_H
