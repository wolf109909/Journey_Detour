//
// Created by scrci on 2022/9/10.
//

#ifndef CL1_UTILS_H
#define CL1_UTILS_H

#include "sharedlibrary.h"

namespace Global {

    typedef __int64(__cdecl *_getconsole)(char *);
    typedef __int64(__cdecl *_printluamem)(__int64);

    extern bool isNetGUIEnabled;
    extern bool console_OutputRedirect;
    namespace Bases {
        //        extern base_t NetGui;
        extern const unsigned long long int GameTick;
        extern const unsigned long long int GetConsole;
        extern const unsigned long long int PrintLuaMem;
        extern const unsigned long long int LuaLoadString;
        extern const unsigned long long int LuaPCall;
        extern const unsigned long long int LuaLoadFilex;
        extern const unsigned long long int LuaToString;
        extern const unsigned long long int LuaGetGlobal;
        extern const unsigned long long int LuaPushValue;
        extern const unsigned long long int LuaCallK;
        extern const unsigned long long int LuaLError;
        extern const unsigned long long int LuaSetTop;
        extern const unsigned long long int LuaClose;
        extern const unsigned long long int LuaPCallK;
        extern const unsigned long long int LuaNewThread;
        extern const unsigned long long int LuaCloseState;
        extern const unsigned long long int LuaDebugDoString;
        extern const unsigned long long int GetTop;
        extern const unsigned long long int DebugPrint;
        extern const unsigned long long int LuaB_print;
        extern const unsigned long long int GetLevelUid;
        extern const unsigned long long int AddText;
        extern const unsigned long long int NetGui;
    }
    typedef const unsigned long long base_t;
}
#endif //CL1_UTILS_H
