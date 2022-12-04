#pragma once
#include "sharedlibrary.h"
#include "render.h"

namespace GUIConsole {
//    extern float GUIConsole_X;
//    extern float GUIConsole_y;
//    extern float GUIConsole_charsize;
//    extern const int GUIConsole_Width;
//    extern const int GUIConsole_Height;
//    extern int CountDown;
//    extern int CountDownLimit;
//    extern int GUIConsole_Cursor;
//    //char GUIConsole_Buffer[GUIConsole_Width * GUIConsole_Height];
//    extern std::vector<std::string> console_lines;

    void AppendLine(std::string line);

    void Draw();
};

//void InitializeLuaGUI();

