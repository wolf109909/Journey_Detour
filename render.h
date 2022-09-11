//
// Created by scrci on 2022/9/10.
//

#ifndef CL1_RENDER_H
#define CL1_RENDER_H

#include "utils.h"
#include "hookutils.h"
#include "game.h"
#include "console.h"
#include "menu.h"

namespace Render {
    typedef int(__cdecl *_addtext)(__int64 a1, const char *text, float x, float y, float size, int color);
    typedef void(__cdecl *_netgui)(__int64 a1, __int64 a2, float a3);

    extern _addtext AddText;
    extern _netgui NetGui;

    int AddTextHook(__int64 a1, const char *text, float x, float y, float size, int color);

    void NetGuiHook(__int64 a1, __int64 a2, float a3);

    void Initialize(HookEnabler hook);
}
#endif //CL1_RENDER_H