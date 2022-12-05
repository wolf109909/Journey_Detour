#include"sharedlibrary.h"


namespace Offsets {

    extern uintptr_t GameTick;
    extern uintptr_t GetConsole;
    extern uintptr_t PrintLuaMem;

    //Lua
    extern uintptr_t LuaLoadString;
    extern uintptr_t LuaPCall;
    extern uintptr_t LuaLoadFilex;
    extern uintptr_t LuaToString;
    extern uintptr_t LuaGetGlobal;
    extern uintptr_t LuaPushValue;
    extern uintptr_t LuaCallK;
    extern uintptr_t LuaLError;
    extern uintptr_t LuaSetTop;
    extern uintptr_t LuaClose;
    extern uintptr_t LuaPCallK;
    extern uintptr_t LuaNewThread;
    extern uintptr_t LuaCloseState;
    extern uintptr_t LuaDebugDoString;
    extern uintptr_t GetTop;
    extern uintptr_t DebugPrint;
    extern uintptr_t LuaB_print;

    //Game
    extern uintptr_t GetLevelUid;

    //Render
    extern uintptr_t AddText;
    extern uintptr_t NetGui;

}
