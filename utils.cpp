//
// Created by scrci on 2022/9/10.
//

#include "utils.h"


bool Global::isNetGUIEnabled = false;
bool Global::console_OutputRedirect = true;

//Shared(Global)
Global::base_t Global::Bases::GameTick           =       0x1400DAD40;
Global::base_t Global::Bases::GetConsole         =       0x140316DC0;
Global::base_t Global::Bases::PrintLuaMem        =       0x1400F0B90;

//Lua
Global::base_t Global::Bases::LuaLoadString      =       0x14031E250;
Global::base_t Global::Bases::LuaPCall           =       0x1403278C0;
Global::base_t Global::Bases::LuaLoadFilex       =       0x14031DEA0;
Global::base_t Global::Bases::LuaToString        =       0x14031B3B0;
Global::base_t Global::Bases::LuaGetGlobal       =       0x14031BA50;
Global::base_t Global::Bases::LuaPushValue       =       0x14031AF30;
Global::base_t Global::Bases::LuaCallK           =       0x14031C340;
Global::base_t Global::Bases::LuaLError          =       0x14031DAA0;
Global::base_t Global::Bases::LuaSetTop          =       0x14031AEE0;
Global::base_t Global::Bases::LuaClose           =       0x14031F6E0;
Global::base_t Global::Bases::LuaPCallK          =       0x14031C3D0;
Global::base_t Global::Bases::LuaNewThread       =       0x14031F700;
Global::base_t Global::Bases::LuaCloseState      =       0x14031FB30;
Global::base_t Global::Bases::LuaDebugDoString   =       0x1402D7460;
Global::base_t Global::Bases::GetTop             =       0x14031AEC0;
Global::base_t Global::Bases::DebugPrint         =       0x1402E3090;
Global::base_t Global::Bases::LuaB_print         =       0x140333040;

//Game
Global::base_t Global::Bases::GetLevelUid        =       0x1400DA830;

//Render
Global::base_t Global::Bases::AddText            =       0x14026EAA0;
Global::base_t Global::Bases::NetGui             =       0x14010DA00;