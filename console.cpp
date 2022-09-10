//
// Created by scrci on 2022/9/10.
//
#define _CRT_SECURE_NO_WARNINGS

#include "console.h"
#include "utils.h"
#include "game.h"

float GUIConsole_X = -1.75;
float GUIConsole_y = 0;
float GUIConsole_charsize = 0.04;
const int GUIConsole_Width = 120;
const int GUIConsole_Height = 30;
int CountDown = 500;
int CountDownLimit = 100;
int GUIConsole_Cursor = 0;
//char GUIConsole_Buffer[GUIConsole_Width * GUIConsole_Height];
std::vector<std::string> console_lines;

void GUIConsole::AppendLine(std::string line) {
    if (console_lines.size() > GUIConsole_Height) {
        console_lines.erase(console_lines.begin());
    }
    CountDown = 500;
    console_lines.push_back(line);
}

void GUIConsole::Draw() {
    if (!Global::console_OutputRedirect)
        return;
    CountDown--;
    int linenum = 0;
    for (const auto& line: console_lines) {
        float x = GUIConsole_X;
        float y = -(linenum * GUIConsole_charsize * 0.75);
        if (CountDown < CountDownLimit) {
            CountDown = 0;
            Render::AddText(Game::GameRender, line.c_str(), x, y, GUIConsole_charsize, 0x30FFFFFF);
        } else {
            Render::AddText(Game::GameRender, line.c_str(), x, y, GUIConsole_charsize, 0xA0FFFFFF);
        }
        //AddText(GameRender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
        linenum++;
    }
}

//void InitializeLuaGUI() {
//    const char *LGUI_DRAW_BOX = "SpawnEvent{HudAddElement = {alpha = 0.33, dimx = 1.3, dimy = .85, duration = .6, elementName = \"Black\", fadeIn = 0, fadeOut = 0.25, posx = 0.13, posy = -0.25, texName = \"Black\", depth = 0, isPauseElem = true}}";
//}