//
// Created by scrci on 2022/9/10.
//
#define _CRT_SECURE_NO_WARNINGS

#include "console.h"
#include "utils.h"

float GUIConsole::GUIConsole_X = -1.75;
float GUIConsole::GUIConsole_y = 0;
float GUIConsole::GUIConsole_charsize = 0.04;
const int GUIConsole::GUIConsole_Width = 120;
const int GUIConsole::GUIConsole_Height = 30;
int GUIConsole::CountDown = 500;
int GUIConsole::CountDownLimit = 100;
int GUIConsole::GUIConsole_Cursor = 0;
//char GUIConsole_Buffer[GUIConsole_Width * GUIConsole_Height];
std::vector<std::string> GUIConsole::console_lines;

void GUIConsole::AppendLine(std::string line) {
    if (console_lines.size() > GUIConsole_Height) {
        console_lines.erase(console_lines.begin());
    }
    CountDown = 500;
    console_lines.push_back(line);
}

void GUIConsole::Draw() {
    if (!Global::redirectconsoleoutput)
        return;
    CountDown--;
    int linenum = 0;
    for (auto line: console_lines) {
        float x = GUIConsole_X;
        float y = -(linenum * GUIConsole_charsize * 0.75);
        if (CountDown < CountDownLimit) {
            CountDown = 0;
            Global::Addtext(Global::gamerender, line.c_str(), x, y, GUIConsole_charsize, 0x30FFFFFF);
        } else {
            Global::Addtext(Global::gamerender, line.c_str(), x, y, GUIConsole_charsize, 0xA0FFFFFF);
        }
        //Addtext(gamerender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
        linenum++;
    }
}
void ConsoleSetup()
{

    AllocConsole();
    SetConsoleTitle("[+] Journey lua hook test");
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    // freopen("CONIN$", "r", stdin);

    HANDLE stdIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;

    if (GetConsoleMode(stdIn, &mode))
    {
        if (mode & ENABLE_QUICK_EDIT_MODE)
        {
            mode &= ~ENABLE_QUICK_EDIT_MODE;
            mode &= ~ENABLE_MOUSE_INPUT;

            mode |= ENABLE_PROCESSED_INPUT;
            SetConsoleMode(stdIn, mode);
        }
    }
    //system("MODE CON COLS=65 LINES=18");
    std::cout << "Injecting..." << std::endl;
    // std::cout << baseAddress << std::endl;
    // std::cout << newthreadptr << std::endl;
}
void InitializeLuaGUI()
{
    const char *LGUI_DRAW_BOX = "SpawnEvent{HudAddElement = {alpha = 0.33, dimx = 1.3, dimy = .85, duration = .6, elementName = \"Black\", fadeIn = 0, fadeOut = 0.25, posx = 0.13, posy = -0.25, texName = \"Black\", depth = 0, isPauseElem = true}}";
}