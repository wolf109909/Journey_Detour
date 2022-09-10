//
// Created by scrci on 2022/9/10.
//
#pragma once
#ifndef CL1_CONSOLE_H
#define CL1_CONSOLE_H
#include "sharedlibrary.h"
namespace GUIConsole
{
    extern float GUIConsole_X;
    extern float GUIConsole_y;
    extern float GUIConsole_charsize;
    extern const int GUIConsole_Width;
    extern const int GUIConsole_Height;
    extern int CountDown;
    extern int CountDownLimit;
    extern int GUIConsole_Cursor;
    //char GUIConsole_Buffer[GUIConsole_Width * GUIConsole_Height];
    extern std::vector<std::string> console_lines;

    void AppendLine(std::string line);
    void Draw();
};
void ConsoleSetup();
void InitializeLuaGUI();
#endif //CL1_CONSOLE_H
