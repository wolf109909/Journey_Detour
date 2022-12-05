#include "offsets.h"
#include "render.h"
#include "vardisplay.h"

Render::_addtext Render::AddText = NULL;
Render::_netgui Render::NetGui = NULL;

int Render::AddTextHook(__int64 a1, const char *text, float x, float y, float size, int color) {
    return Render::AddText(a1, text, x, y, size, color);
}

// no reference,define as local function
void CustomTextDoRender() {
    GUIConsole::Draw();
    Menu::GUI::Draw();
    VarDisplay::Draw();
}

void Render::NetGuiHook(__int64 a1, __int64 a2, float a3) {
    // add custom text render barn here cuz the game will call it while queueing render is possible
    CustomTextDoRender();
    if(Game::Render == 0)
        return NetGui(a1, a2, a3);
    Render::AddText(Game::Render, "Journey Detour v1.0.1", -1.75, -1.0, 0.05, 0xFFFFFFFF);
    //AddText(GameRender, " <9002>", -1.5, -0.9, 0.05, 0xFF0000FF);

    //int result = AddText(gamestructbase + 192, "TESTTEST", (float)-0.1, (float)-0.1, (float)0.5, 136);
    //std::cout << result << std::endl;
    NetGui(a1, a2, a3);
}

void Render::Initialize(HookEnabler hook) {
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Offsets::AddText, &Render::AddTextHook, reinterpret_cast<LPVOID *>(&Render::AddText));
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Offsets::NetGui, &Render::NetGuiHook, reinterpret_cast<LPVOID *>(&Render::NetGui));
}