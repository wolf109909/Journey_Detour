#include "pch.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "MinHook.h"
#include <Psapi.h>
#include <thread>
#include <chrono>
#include "lua.h"
#include "game.h"
#include "render.h"
#include "menu.h"

bool m_bCommandFound = false;
HANDLE consoleInputThreadHandle = NULL;

char m_cConsoleBuffer[24];
bool m_bDebugLoopEnabled = false;

void PrintLuaMem() { 
    if (g_pGame->g_GetConsole == nullptr)
    {
        spdlog::error("[FATAL] Attempt to call PrintLuaMem when g_GetConsole is not initialized!");
        return;
    }
    while (m_bDebugLoopEnabled) {
        __int64 consolehandle = g_pGame->g_GetConsole(m_cConsoleBuffer);
        g_pGame->g_PrintLuaMem(consolehandle);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(200ms);
    }
    return;
}


DWORD WINAPI ConsoleInputThread(PVOID pThreadParameter) {

    FILE* fp = nullptr;
    freopen_s(&fp, "CONIN$", "r", stdin);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2000ms);
    std::cout << R"(
---------------------------------------------------------------------------
       __                                  ____       __
      / ____  __  ___________  ___  __  __/ __ \___  / /_____  __  _______
 __  / / __ \/ / / / ___/ __ \/ _ \/ / / / / / / _ \/ __/ __ \/ / / / ___/
/ /_/ / /_/ / /_/ / /  / / / /  __/ /_/ / /_/ /  __/ /_/ /_/ / /_/ / /
\____/\____/\__,_/_/  /_/ /_/\___/\__, /_____/\___/\__/\____/\__,_/_/
                                 /____/

              https://github.com/wolf109909/Journey_Detour
--------------------------------------------------------------------------
)" << std::endl;

    std::cout << "     +===========================HELP============================+" << std::endl;
    std::cout << "     | meminfo(F3)     : Toggle memory info prints.              |" << std::endl;
    std::cout << "     | debug(F1)       : Run remove debugger function.           |" << std::endl;
    std::cout << "     | debug <filename>: Debug target lua file.                  |" << std::endl;
    std::cout << "     | debughud(F8)    : Cycle through DebugHud.                 |" << std::endl;
    std::cout << "     | script <luacode>: Force Execute lua code to VM.(May crash)|" << std::endl;
    std::cout << "     | console(F7)     : Toggle console redirect                 |" << std::endl;
    std::cout << "     | help            : Print this screen again                 |" << std::endl;
    std::cout << "     | clear           : Clear the console                       |" << std::endl;
    std::cout << "     +===========================================================+" << std::endl;
    std::cout << "\n\n[*] Ready to receive console commands." << std::endl;

    {
        // Process console input
        std::string input;
        while (std::getline(std::cin, input)) {
            auto pos = input.find(' ');
            std::string command;
            if (pos != std::string::npos) {
                command = input.substr(0, pos);
            }
            else {
                command = input;
            }

            if (command == "script") {
                try {
                    input = input.substr(7);
                }
                catch (std::out_of_range&) {
                    std::cout << "script error: no input given!" << std::endl;
                    continue;
                }

                g_pLua->AppendBuffer(input);

                m_bCommandFound = true;
            }

            if (command == "clear") {
                system("cls");
                m_bCommandFound = true;
            }

            if (command == "queuelevel") {
                try {
                    input = input.substr(11);
                }

                catch (std::out_of_range&) {
                    std::cout << "queuelevel error: no input given!" << std::endl;
                    continue;
                }

                input = "game:QueueLevel(\"" + input.substr(0, input.size() - 1) + "\")";

                std::cout << input << std::endl;
                g_pLua->AppendBuffer(input);
                input.clear();
                m_bCommandFound = true;
            }

            if (command == "netgui") {
                //luaL_loadstring_f(new_lua_State_ptr, "game:netGui():ToggleEnabled()") || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
                g_pLua->AppendBuffer("game:netGui():ToggleEnabled()");
                g_pGame->b_IsNetGUIEnabled = !g_pGame->b_IsNetGUIEnabled;
                input.clear();
                m_bCommandFound = true;
            }

            if (command == "addtext") {
                try {
                    input = input.substr(8);
                }
                catch (std::out_of_range&) {
                    std::cout << "addtext error: no input given!" << std::endl;
                    continue;
                }

                //GUIConsole::AppendLine(input);

                m_bCommandFound = true;
            }

            if (command == "test") {
                std::cout << "Test function called!" << std::endl;

                input.clear();
                m_bCommandFound = true;
            }

            if (command == "console") {
                if (g_pGame->b_IsConsoleRedirectEnabled) {
                    std::cout << "[*] Disabled game print redirection" << std::endl;
                }
                else {
                    std::cout << "[*] Enabled game print redirection" << std::endl;
                }
                g_pGame->b_IsConsoleRedirectEnabled = !g_pGame->b_IsConsoleRedirectEnabled;
                m_bCommandFound = true;
            }
            if (command == "help") {
                std::cout << "     +===========================HELP============================+" << std::endl;
                std::cout << "     | meminfo(F3)     : Toggle memory info prints.              |" << std::endl;
                std::cout << "     | debug(F1)       : Run remove debugger function.           |" << std::endl;
                std::cout << "     | debug <filename>: Debug target lua file.                  |" << std::endl;
                std::cout << "     | debughud(F8)    : Cycle through DebugHud.                 |" << std::endl;
                std::cout << "     | script <luacode>: Force Execute lua code to VM.(May crash)|" << std::endl;
                std::cout << "     | console(F7)     : Toggle console redirect                 |" << std::endl;
                std::cout << "     | help            : Print this screen again                 |" << std::endl;
                std::cout << "     | clear           : Clear the console                       |" << std::endl;
                std::cout << "     +===========================================================+" << std::endl;


                m_bCommandFound = true;
            }
            if (command == "debughud") {
                g_pLua->AppendBuffer("game:debugHud():CycleMode()");
                //luaL_loadstring_f(new_lua_State_ptr, "game:debugHud():CycleMode()");
                //lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);

                std::cout << "[!] Switching debugHud mode." << std::endl;
                m_bCommandFound = true;
            }
            if (command == "debug") {
                std::string cmd;
                if (input.length() > 5) {
                    input = input.substr(6);
                    cmd = "debugtarget=\"" + input + "\");debugmode=2";
                }
                else {
                    cmd = "debugmode=1";
                }


                g_pLua->AppendBuffer(cmd.c_str());

                std::cout << "[+] Waiting for debugger..." << std::endl;

                input.clear();
                m_bCommandFound = true;
            }

            if (command == "meminfo") {
                m_bDebugLoopEnabled = !m_bDebugLoopEnabled;
                m_bCommandFound = true;
            }

            if (!m_bCommandFound) {
                std::cout << "[-] Command not found!" << std::endl;
                m_bCommandFound = false;
            }
        }
    }

    return 0;
}
void ConsoleSetup()
{

    AllocConsole();
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
    spdlog::set_pattern("[%^%l%$]%v");
    spdlog::info("Console Initialized.");
    // std::cout << baseAddress << std::endl;
    // std::cout << newthreadptr << std::endl;
}
int WINAPI main() {
    ConsoleSetup();
    InstallInitialHooks();
    g_pLua->Initialize();
    g_pGame->Initialize();
    RenderTest::Initialize();
    Menu::Initialize();
    consoleInputThreadHandle = CreateThread(0, 0, ConsoleInputThread, 0, 0, NULL);
    //g_pLua->AppendBuffer("os.execute(\"pause\")");



    while (true) {
        int status;

        if (GetAsyncKeyState(VK_UP) & 1) {
            Menu::SelectPreviousItem();

            //luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
        }

        if (GetAsyncKeyState(VK_DOWN) & 1) {
            Menu::SelectNextItem();

            //luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
        }

        if (GetAsyncKeyState(VK_END) & 1) {
            Menu::ExecuteItem();

            //luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
        }

        if (GetAsyncKeyState(VK_RETURN) & 1 && g_pGame->b_IsNetGUIEnabled) {
            g_pLua->AppendBuffer("game:netGui():ExecuteSelectedItem(game)");

            //luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
        }


        if (GetAsyncKeyState(VK_F1) & 1) {
            std::cout << "[*] Waiting for debugger..." << std::endl;
            g_pLua->AppendBuffer("debugmode=1");
        }
        if (GetAsyncKeyState(VK_F3) & 1) {
            if (m_bDebugLoopEnabled) {
                std::cout << "[*] Disabled Memory profiler" << std::endl;
            }
            else {
                // initialize meminfo thread
                std::thread thread_debugInfo(PrintLuaMem);
                thread_debugInfo.detach();
                std::cout << "[*] Enabled Memory profiler" << std::endl;
            }
            m_bDebugLoopEnabled = !m_bDebugLoopEnabled;

        }
        if (GetAsyncKeyState(VK_F7) & 1) {
            if (g_pGame->b_IsConsoleRedirectEnabled) {
                std::cout << "[*] Disabled game print redirection" << std::endl;
            }
            else {
                std::cout << "[*] Enabled game print redirection" << std::endl;
            }
            g_pGame->b_IsConsoleRedirectEnabled = !g_pGame->b_IsConsoleRedirectEnabled;

        }


        if (GetAsyncKeyState(VK_F8) & 1) {
            g_pLua->AppendBuffer("game:debugHud():CycleMode()");
            std::cout << "[!] Switching debugHud mode." << std::endl;
        }
        Sleep(100);
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:{
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

