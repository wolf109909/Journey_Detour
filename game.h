#pragma once
#include "sharedlibrary.h"
#include "hookutils.h"

namespace Game {
    typedef void(__cdecl *_tick)(__int64 a1, float a2);
//    typedef int GetLevelUidType(Lua::lua_State *, char *LevelName);
    //uintptr_t L;
    extern uintptr_t        BaseGame;
    extern uintptr_t        Render;
    extern uintptr_t        Matchmaker;
    extern uintptr_t        PlayerBarn;
    //extern uintptr_t        LocalDude;
    extern uintptr_t        LuaState;
    extern _tick            Tick;

//    extern GetLevelUidType  *GetLevelUid;
    extern bool             locateGame;
    extern bool*             autolobbybool1;
    extern bool*            autolobbybool2;
    extern unsigned __int8  someinteger;


    void UpdateValues();

    char *CurrentLevelName();

    int CurrentLevelUid();

    void SetBaseAddress(uintptr_t addr);

    void Initialize(HookEnabler hook);

    namespace LocalDude 
    {
        extern float* XPos;
        extern float* YPos;
        extern float* ZPos;

        extern float* XAccel;
        extern float* YAccel;
        extern float* ZAccel;

        extern int* ScarfCharge;
        extern int* ScarfMax;
    };
};

