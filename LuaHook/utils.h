//
// Created by scrci on 2022/9/10.
//

#ifndef CL1_UTILS_H
#define CL1_UTILS_H

#include "sharedlibrary.h"

namespace Global{
    typedef int(__cdecl* _addtext)(__int64 a1, const char* text, float x, float y, float size, int color);
    extern _addtext Addtext;
    extern uintptr_t gamerender;
    extern uintptr_t BaseGame;
    extern uintptr_t BaseRender;
    extern uintptr_t BaseMatchmaker;
    extern bool isNetGUIEnabled;
    extern bool redirectconsoleoutput;



}
#endif //CL1_UTILS_H
