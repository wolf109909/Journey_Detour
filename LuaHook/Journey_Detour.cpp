// LuaInjector.cpp : Defines the exported functions for the DLL application.
//
// Standard imports
#include "stdafx.h"
#include "hookutils.h"
#include "luahooks.h"
#include "game.h"
#include "render.h"
#include "custommenu.h"
#include "guiconsole.h"

//#include "Lua.h"
//#include "game.h"
//#include "custommenu.h"
//#include "guiconsole.h"
//#include "render.h"
//#pragma comment(lib, "lua53.lib")
#if defined _M_X64
#pragma comment(lib, "libMinHook-x64-v141-mdd.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mdd.lib")
#endif


typedef __int64(__cdecl* _getconsole)(char*);
_getconsole getconsole_f = (_getconsole)0x140316DC0;

typedef __int64(__cdecl* _printluamem)(__int64);
_printluamem printluamem_f = (_printluamem)0x1400F0B90;


typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);

bool commandfound = false;
HANDLE consoleInputThreadHandle = NULL;
bool debugloop = false;
DWORD MemInfoConsolePid;
char consolebuffer[24];

void PollLuaMem()
{
	while (true)
	{
		if (debugloop)
		{
			__int64 consolehandle = getconsole_f(consolebuffer);
			printluamem_f(consolehandle);
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(200ms);
		}
	}
}

DWORD WINAPI ConsoleInputThread(PVOID pThreadParameter)
{
	
	FILE *fp = nullptr;
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

	// initialize meminfo thread
	std::thread debuginfothread(PollLuaMem);
	debuginfothread.detach();

	{
		using namespace Lua;
		// Process console input
		std::string input;
		while (std::getline(std::cin, input))
		{
			auto pos = input.find(' ');
			std::string command;
			if (pos != std::string::npos)
			{
				command = input.substr(0, pos);
			}
			else
			{
				command = input;
			}

			if (command == "script")
			{
				try
				{
					input = input.substr(7);
				}
				catch (std::out_of_range &)
				{
					std::cout << "script error: no input given!" << std::endl;
					continue;
				}

				Lua::AppendBuffer(input);

				commandfound = true;
			}

			if (command == "clear")
			{
				system("cls");
				commandfound = true;
			}

			if (command == "queuelevel")
			{
				try
				{
					input = input.substr(11);
				}

				catch (std::out_of_range &)
				{
					std::cout << "queuelevel error: no input given!" << std::endl;
					continue;
				}

				input = "game:QueueLevel(\"" + input.substr(0, input.size() - 1) + "\")";

				std::cout << input << std::endl;
				Lua::AppendBuffer(input);
				input.clear();
				commandfound = true;
			}

			if (command == "netgui")
			{
				//luaL_loadstring_f(new_lua_State_ptr, "game:netGui():ToggleEnabled()") || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
				Lua::AppendBuffer("game:netGui():ToggleEnabled()");
				Game::isNetGUIEnabled = !Game::isNetGUIEnabled;
				input.clear();
				commandfound = true;
			}

			if (command == "addtext")
			{
				try
				{
					input = input.substr(8);
				}
				catch (std::out_of_range&)
				{
					std::cout << "addtext error: no input given!" << std::endl;
					continue;
				}

				GUIConsole::AppendLine(input);

				commandfound = true;
			}

			if (command == "test")
			{
				std::cout << "Test function called!" << std::endl;
				commandfound = true;
			}

			if (command == "console") 
			{
				GUIConsole::Toggle();
				commandfound = true;
			}
			if (command == "help")
			{
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
				
				

				commandfound = true;
			}
			if (command == "debughud")
			{
				Lua::AppendBuffer("game:debugHud():CycleMode()");
				//luaL_loadstring_f(new_lua_State_ptr, "game:debugHud():CycleMode()");
				//lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
				
				std::cout << "[!] Switching debugHud mode." << std::endl;
				commandfound = true;
			}
			if (command == "debug")
			{
				std::string cmd;
				if (input.length() > 5)
				{
					input = input.substr(6);
					cmd = "debugtarget=\"" + input + "\");debugmode=2";
				}
				else
				{
					cmd = "debugmode=1";
				}


				Lua::AppendBuffer(cmd.c_str());
				
				std::cout << "[+] Waiting for debugger..." << std::endl;

				input.clear();
				commandfound = true;
			}

			if (command == "meminfo") {
				debugloop = !debugloop;
				commandfound = true;
			}

			if (commandfound == false)
			{
				std::cout << "[-] Command not found!" << std::endl;
				commandfound = false;
			}
		}
	}

	return 0;
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

//gettop newFunction = NULL;







// Get all module related info, this will include the base DLL.
// and the size of the module
MODULEINFO GetModuleInfo(char *szModule)
{
	MODULEINFO modinfo = {0};
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

// Pattern Scan https://guidedhacking.com/threads/c-signature-scan-pattern-scanning-tutorial-difficulty-3-10.3981
uintptr_t FindPattern(char *module, char *pattern, char *mask)
{
	// Get all module related information
	MODULEINFO mInfo = GetModuleInfo(module);

	// Assign our base and module size
	uintptr_t base = (uintptr_t)mInfo.lpBaseOfDll;

	uintptr_t size = (uintptr_t)mInfo.SizeOfImage;

	// Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			// if we have a ? in our mask then we have true by default,
			// or if the bytes match then we keep searching until finding it or not
			found &= mask[j] == '?' || pattern[j] == *(char *)(base + i + j);
		}

		// found = true, our entire pattern was found
		if (found)
		{
			std::cout << "[+] Pattern found: " << base + i << std::endl;
			return base + i;
		}
	}
	std::cout << "[-] Pattern not found" << std::endl;
	return NULL;
}








int refKey = 0;

int WINAPI main()
{
	using namespace Lua;
	ConsoleSetup();

	// Initialize MinHook
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	// initialize Game with other offsets
	Game::Initialize();
	Render::Initialize();
	GUIConsole::Initialize();
	MenuSystem::InitializeMenu();






	// to juesto: uncomment this if you want to try running code from a new luastate. also change the two function calls using lua_state_ptr to new_ptr.
	//new_lua_State_ptr = Lua::CreateThread();
	// luaL_loadfilex_f(new_lua_State_ptr, "main.lua", NULL) || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);

	consoleInputThreadHandle = CreateThread(0, 0, ConsoleInputThread, 0, 0, NULL);

	//Lua::AppendBuffer("Vars.Game.cheatsEnabled(true)");

	MenuSystem::InitializeMenu();

	while (true)
	{
		int status;

		if (GetAsyncKeyState(VK_UP) & 1)
		{
			MenuSystem::SelectPreviousItem();

			//luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
		}

		if (GetAsyncKeyState(VK_DOWN) & 1)
		{
			MenuSystem::SelectNextItem();

			//luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
		}

		if (GetAsyncKeyState(VK_END) & 1)
		{
			MenuSystem::ExecuteItem();

			//luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
		}

		if (GetAsyncKeyState(VK_RETURN) & 1 && Game::isNetGUIEnabled)
		{
			Lua::AppendBuffer("game:netGui():ExecuteSelectedItem(game)");

			//luaL_loadstring_f(new_lua_State_ptr, ) || lua_pcallk_f(new_lua_State_ptr, 0, 0, 0, 0, NULL);
		}


		if (GetAsyncKeyState(VK_F1) & 1)
		{
			std::cout << "[*] Waiting for debugger..." <<std::endl;
			Lua::AppendBuffer("debugmode=1");
		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			if (debugloop)
			{
				std::cout << "[*] Disabled Memory profiler" << std::endl;
			}
			else
			{
				std::cout << "[*] Enabled Memory profiler" << std::endl;
			}
			debugloop = !debugloop;
			std::thread debuginfothread(PollLuaMem);
			debuginfothread.detach();

		}
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			GUIConsole::Toggle();
		}


		if (GetAsyncKeyState(VK_F8) & 1)
		{
			Lua::AppendBuffer("game:debugHud():CycleMode()");
			std::cout << "[!] Switching debugHud mode." << std::endl;
		}
		Sleep(100);
	}
	return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
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
