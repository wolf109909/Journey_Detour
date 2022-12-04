#include "sharedlibrary.h"

class Offsets{
    public:

    uintptr_t GameTick           =       0x1400DAD40;
    uintptr_t GetConsole         =       0x140316DC0;
    uintptr_t PrintLuaMem        =       0x1400F0B90;

    //Lua
    uintptr_t LuaLoadString      =       0x14031E250;
    uintptr_t LuaPCall           =       0x1403278C0;
    uintptr_t LuaLoadFilex       =       0x14031DEA0;
    uintptr_t LuaToString        =       0x14031B3B0;
    uintptr_t LuaGetGlobal       =       0x14031BA50;
    uintptr_t LuaPushValue       =       0x14031AF30;
    uintptr_t LuaCallK           =       0x14031C340;
    uintptr_t LuaLError          =       0x14031DAA0;
    uintptr_t LuaSetTop          =       0x14031AEE0;
    uintptr_t LuaClose           =       0x14031F6E0;
    uintptr_t LuaPCallK          =       0x14031C3D0;
    uintptr_t LuaNewThread       =       0x14031F700;
    uintptr_t LuaCloseState      =       0x14031FB30;
    uintptr_t LuaDebugDoString   =       0x1402D7460;
    uintptr_t GetTop             =       0x14031AEC0;
    uintptr_t DebugPrint         =       0x1402E3090;
    uintptr_t LuaB_print         =       0x140333040;

    //Game
    uintptr_t GetLevelUid        =       0x1400DA830;

    //Render
    uintptr_t AddText            =       0x14026EAA0;
    uintptr_t NetGui             =       0x14010DA00;

};
extern Offsets* g_Offsets;