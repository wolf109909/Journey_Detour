// LuaInjector.cpp : Defines the exported functions for the DLL application.
//
// Standard imports
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "MinHook.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include <thread>
#include <chrono>
#include <random>
#include <functional>
#include "hookutils.h"
#include "spdlog/spdlog.h"
#include <map>
//#pragma comment(lib, "lua53.lib")
#if defined _M_X64
#pragma comment(lib, "libMinHook-x64-v141-mdd.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mdd.lib")
#endif



// Function definitions

typedef void(__cdecl* _phyreprint)(unsigned int severity, const char* a2, ...);
typedef void(__cdecl* _netgui)(__int64 a1, __int64 a2, float a3);
typedef void(__cdecl* _gametick)(__int64 a1, float a2 );
typedef int(__cdecl* _addtext)(__int64 a1, const char* text, float x, float y, float size, int color);

typedef __int64(__cdecl *_getconsole)(char *);
typedef __int64(__cdecl *_printluamem)(__int64);

typedef void logPointer(std::string name, uint64_t pointer);



// Function bindings

//_debug_print debug_print = (_debug_print)0x0;

_netgui BaseNetGui = (_netgui)0x14010DA00;
_gametick BaseGameTick = (_gametick)0x1400DAD40;
//_addtext Addtext = (_addtext)0x14026EAA0;
_getconsole getconsole_f = (_getconsole)0x140316DC0;
_printluamem printluamem_f = (_printluamem)0x1400F0B90;



typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);


bool isNetGUIEnabled = false;
bool commandfound = false;
HANDLE consoleInputThreadHandle = NULL;
// const char* Console_luaScriptCommandName = "script";


bool debugloop = false;
bool redirectconsoleoutput = true;


DWORD MemInfoConsolePid;
_addtext Addtext = NULL;

_netgui origNetGuiFunction = NULL;
_phyreprint origPhyrePrintFunction = NULL;
_gametick origGameTickFunction = NULL;

uint64_t baseAddress = 0;
uintptr_t gamerender = 0;
uintptr_t gamestructbase = 0;
uintptr_t gamenetwork = 0;

char consolebuffer[24];
// y: -1 ~ 1
// x: -(16/9) ~ (16/9)
// in game console definition
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

namespace Lua
{

	#define lua_writestring(s, l) fwrite((s), sizeof(char), (l), stdout)
	#define lua_writeline() (lua_writestring("\n", 1), fflush(stdout))
	#define LUA_KCONTEXT intptr_t
	#define lua_call(L, n, r) lua_callk(L, (n), (r), 0, NULL)
	#define lua_pop(L, n) lua_settop(L, -(n)-1)
	typedef intptr_t lua_KContext;
	typedef void* lua_State;
	typedef void(__cdecl* _lua_close)(lua_State* L);
	typedef void(__cdecl* _debug_print)(const char* a1, ...);
	typedef int(__cdecl* _luaB_print)(lua_State*);
	typedef int(__cdecl* gettop)(lua_State*);
	typedef lua_State* (__cdecl* newThread)(lua_State*);
	typedef void(__cdecl* pushinteger)(lua_State*, int);
	typedef int(__cdecl* tointeger)(lua_State*, int);
	typedef int(__cdecl* _luaStatus)(lua_State*);
	typedef int(__cdecl* _luaL_loadstring)(lua_State*, const char*);
	typedef const char* (__cdecl* lua_tostring_)(lua_State*, int, size_t*);
	typedef int(__cdecl* _lua_getglobal)(lua_State*, const char*);
	typedef void(__cdecl* _lua_pushvalue)(lua_State*, int);
	typedef float(__cdecl* _drawdebugtext)(uintptr_t, const char*, float, uintptr_t, float, int);
	typedef int(__cdecl* _luaL_error)(lua_State*, const char*, ...);
	typedef void(__cdecl* _lua_settop)(lua_State*, int);
	typedef int (*_lua_debugdostring)(lua_State*, const char*);
	typedef int (*_luaL_loadfilex)(lua_State* L, const char* filename, const char* mode);
	typedef int (*_lua_pcall)(lua_State* L, int nargs, int nresults, int errfunc);
	typedef int (*lua_KFunction)(lua_State* L, int status, intptr_t ctx);
	typedef int (*_close_state)(lua_State* L);
	typedef int (*_lua_pcallk)(lua_State* L, int nargs, int nresults, int errfunc, intptr_t ctx, lua_KFunction k);
	typedef void(__cdecl* _lua_callk)(lua_State*, int, int, intptr_t ctx, lua_KFunction);
	typedef int luaL_loadbuffer_(lua_State* L, char* buff, size_t size, char* name);
	typedef int lua_pcall_(lua_State* L, int nargs, int nresults, int errfunc);

	// typedef const char* lua_tostring_(lua_State* L, int32_t idx);
	typedef uint32_t lua_isstring_(lua_State* L, int32_t idx);
	typedef lua_State* lua_newthread_(lua_State* L);

	_luaL_loadstring luaL_loadstring = (_luaL_loadstring)0x14031E250;
	_lua_pcall lua_pcall = (_lua_pcall)0x1403278C0;
	_luaL_loadfilex luaL_loadfilex = (_luaL_loadfilex)0x14031DEA0;
	lua_tostring_ lua_tostring = (lua_tostring_)0x14031B3B0;
	_lua_getglobal lua_getglobal = (_lua_getglobal)0x14031BA50;
	_lua_pushvalue lua_pushvalue = (_lua_pushvalue)0x14031AF30;
	_lua_callk lua_callk = (_lua_callk)0x14031C340;
	_luaL_error luaL_error = (_luaL_error)0x14031DAA0;
	_lua_settop lua_settop = (_lua_settop)0x14031AEE0;
	_lua_close lua_close = (_lua_close)0x14031F6E0;
	_lua_pcallk lua_pcallk= (_lua_pcallk)0x14031C3D0;
	newThread lua_newthread = (newThread)0x14031F700;
	uintptr_t luab_print_ptr = 0x140333040;
	_close_state close_state = (_close_state)0x14031FB30;
	_lua_debugdostring lua_debugdostring = (_lua_debugdostring)0x1402D7460;

	std::vector<std::string> m_LuaExecuteBuffer;
	lua_State* lua_State_ptr = 0;
	lua_State* new_lua_State_ptr = 0;
	_debug_print origTargetdebugprintFunction = NULL;
	gettop origTargetFunction = NULL;
	_luaB_print origLuaBPrintFunction = NULL;
	bool luaStateObtained = false;
	int luastatus;

	int _gettop(lua_State* L)
	{
		// std::cout << "[!] Hook called :O" << std::endl;
		if (lua_State_ptr == 0)
		{
			lua_State_ptr = (lua_State*)L;
			// std::cout << "[+] State Obtained: \t" << std::hex << lua_State_ptr << std::endl;
			spdlog::info("Successfully obtained state:{}", *lua_State_ptr);
			if (MH_DisableHook((LPVOID)0x14031AEC0) != MH_OK)
			{
				std::cout << "[+] Hook disabled" << std::endl;
			}
		}
		luaStateObtained = true;

		// return (*(DWORD*)(state + 0x18) - *(DWORD*)(state + 0x10)) >> 3;
		return origTargetFunction(L);
	}

	int luaB_print_f(lua_State* L)
	{
		int n = origTargetFunction(L); /* number of arguments */
		int i;
		lua_getglobal(L, "tostring");
		for (i = 1; i <= n; i++)
		{
			const char* s;
			size_t l;
			lua_pushvalue(L, -1); /* function to be called */
			lua_pushvalue(L, i);	/* value to print */
			lua_call(L, 1, 1);
			s = lua_tostring(L, -1, &l); /* get result */
			if (s == NULL)
				return luaL_error(L, "'tostring' must return a string to 'print'");
			if (i > 1)
				lua_writestring("\t", 1);
			lua_writestring(s, l);
			lua_pop(L, 1); /* pop result */
		}
		lua_writeline();
		// origLuaBPrintFunction(new_lua_State_ptr);
		return 0;
	}

	lua_State* CreateThread()
	{
		spdlog::info("Creating new thread");
		// std::cout << "[+] Calling newThread" << std::endl;
		lua_State* thread = lua_newthread(lua_State_ptr);
		// std::cout << "[+] New LuaState: " << thread << std::endl;
		// lua_pop(lua_State_ptr, 1);
		return thread;
	}
	void AppendBuffer(std::string buffer)
	{
		//std::cout << buffer << std::endl;
		m_LuaExecuteBuffer.push_back(buffer);
	}

	void ExecuteBuffer()
	{
		if (m_LuaExecuteBuffer.size() == 0)
			return;

		for (auto buffer : m_LuaExecuteBuffer)
		{
			luaL_loadstring(new_lua_State_ptr, buffer.c_str());
			if (lua_pcallk(new_lua_State_ptr, 0, 0, 0, 0, NULL) != 0)
			{
				std::cout << "ERROR: " << lua_tostring(new_lua_State_ptr, -1, NULL) << std::endl;
			}
			//std::cout << "buffer executed" << std::endl;
			m_LuaExecuteBuffer.erase(m_LuaExecuteBuffer.begin());
		}

	}

};

namespace Game
{
	using namespace Lua;
	uintptr_t game;
	uintptr_t render;
	uintptr_t matchmaker;
	//uintptr_t L;
	typedef int GetLevelUidType(lua_State*, char* LevelName);
	GetLevelUidType* GetLevelUid = (GetLevelUidType*)0x1400DA830;
	bool locateGame = false;
	bool autolobbybool1;
	bool autolobbybool2;
	unsigned __int8 someinteger;

	void UpdateValues()
	{
		render = *(int*)(game + 192);
		matchmaker = *(int*)(game + 384);
		someinteger = *(unsigned __int8*)(matchmaker + 1536);
		autolobbybool1 = *(bool*)(someinteger + 1057);
		autolobbybool2 = *(bool*)(someinteger + 1058);

	}

	char* CurrentLevelName()
	{
		return (char*)(game + 1835760);
	}
	int CurrentLevelUid()
	{
		return GetLevelUid(lua_State_ptr, CurrentLevelName());
	}
	void SetBaseAddress(uintptr_t addr)
	{
		if (!locateGame)
		{
			game = addr;
			UpdateValues();
		}

	}

};

namespace GUIConsole
{
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

	void AppendLine(std::string line)
	{
		if (console_lines.size() > GUIConsole_Height)
		{
			console_lines.erase(console_lines.begin());
		}
		CountDown = 500;
		console_lines.push_back(line);
	}
	void Draw()
	{
		if (!redirectconsoleoutput)
			return;

		CountDown--;
		int linenum = 0;
		for (auto line : console_lines)
		{
			float x = GUIConsole_X;
			float y = -(linenum * GUIConsole_charsize * 0.75);
			if (CountDown < CountDownLimit)
			{
				CountDown = 0;
				Addtext(gamerender, line.c_str(), x, y, GUIConsole_charsize, 0x30FFFFFF);
			}
			else
			{
				Addtext(gamerender, line.c_str(), x, y, GUIConsole_charsize, 0xA0FFFFFF);
			}

			//Addtext(gamerender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
			linenum++;
		}

	}
};

namespace MenuSystem
{
	typedef void (*Action)(void); // function pointer type

	class Item
	{
	private:
		std::string ItemName;
		Action function;
	public:
		Item(std::string name, Action function)
		{
			this->ItemName = name;
			this->function = function;
		}
		void Execute()
		{
			this->function();
		}
		std::string GetName()
		{
			return this->ItemName;
		}
	};

	int selectedItem = 0;
	std::map<int, Item*> MenuItems;



	void MenuSelectionChanged()
	{
		if (selectedItem < 0)
			selectedItem = 0;
		if (selectedItem > MenuItems.size() - 1)
			selectedItem = MenuItems.size() - 1;
	}
	void SelectPreviousItem()
	{
		selectedItem--;
		MenuSelectionChanged();
	}

	void SelectNextItem()
	{
		selectedItem++;
		MenuSelectionChanged();
	}

	void ExecuteItem()
	{
		Item item = *MenuItems[selectedItem];
		item.Execute();
	}

	void AddItem(std::string itemname, Action function)
	{
		Item* item = new Item(itemname, function);
		int id = MenuItems.size();
		spdlog::info("Adding menu item {},id {}", itemname, id);
		MenuItems.insert_or_assign(id, item);
	}

	void Action_Test()
	{
		Lua::AppendBuffer("game:QueueLevel(\"Level_Desert\")");
	}

	void Action_ToggleNetGui()
	{
		Lua::AppendBuffer("game:netGui():ToggleEnabled()");
		isNetGUIEnabled = !isNetGUIEnabled;
	}
	void Action_ToggleConsole()
	{
		redirectconsoleoutput = !redirectconsoleoutput;
	}
	void Action_CycleDebugHud()
	{
		Lua::AppendBuffer("game:debugHud():CycleMode()");
	}
	void Action_SetMaxOutfit()
	{
		Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetOutfit(8)");
	}
	void Action_SetMaxCloth() 
	{
		Lua::AppendBuffer("SpawnEvent{ SetMaxCloth = { changeAmount = 32 } }");
	}
	void Action_FillScarf()
	{
		Lua::AppendBuffer("game:playerBarn():GetLocalDude():FillScarf(32, game:soundBarn())");
	}
	void Action_GotoNick()
	{
		Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetPos(game:playerBarn():GetRemoteDude():GetPos())");
	}
	void Action_ToggleMusic()
	{
		Lua::AppendBuffer("game:soundBarn():ToggleMuteMusic()");
	}
	


	void InitializeMenu() 
	{
		AddItem("ToggleNetGui", &Action_ToggleNetGui);
		AddItem("ToggleConsole", &Action_ToggleConsole);
		AddItem("CycleDebugHud", &Action_CycleDebugHud);
		AddItem("SetMaxOutfit", &Action_SetMaxOutfit);
		AddItem("SetMaxCloth", &Action_SetMaxCloth);
		AddItem("FillScarf", &Action_FillScarf);
		AddItem("GotoNick", &Action_GotoNick);
		AddItem("ToggleMusic", &Action_ToggleMusic);
		
	}

	namespace GUI 
	{
		float xoffset = 1.2;
		float yoffset = 0.75;
		float fontsize = 0.05;
		unsigned int color = 0xFFFFFF00;
		unsigned int color_picked = 0xFF0000FF;

		void Draw()
		{
			for (auto mapitem : MenuItems)
			{
				//Item item = *item;
				int itemid = mapitem.first;
				Item item = *mapitem.second;
				float x = xoffset;
				float y = yoffset - fontsize*itemid;
				std::string itemname = item.GetName();
				//spdlog::info(item.GetName().c_str());
				if (itemid == selectedItem)
				{
					Addtext(gamerender, itemname.c_str(), x, y, fontsize, color_picked);
				}
				else
				{
					Addtext(gamerender, itemname.c_str(), x, y, fontsize, color);
				}
				//spdlog::info("X: {} Y:{} NAME: {} SIZE: {} COLOR: {}", x, y, itemname, fontsize, color);
				//Sleep(1000);
				//Addtext(gamerender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
			}

		}
	}
};


void initializeInGameLogging()
{
	
}

void InitializeLuaGUI()
{
	const char *LGUI_DRAW_BOX = "SpawnEvent{HudAddElement = {alpha = 0.33, dimx = 1.3, dimy = .85, duration = .6, elementName = \"Black\", fadeIn = 0, fadeOut = 0.25, posx = 0.13, posy = -0.25, texName = \"Black\", depth = 0, isPauseElem = true}}";
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
				isNetGUIEnabled = !isNetGUIEnabled;
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
				// uintptr_t debugtexta1 = 0x1400D3940;
				// uintptr_t debugtexty = 0x1400A2E90;
				// int debugtexta5 = 0xFFFFFF;

				//	drawdebugtext_f(debugtexta1, "Debug Print Test!!!",1, debugtexty ,1, debugtexta5);

				int stacksize = origTargetFunction(new_lua_State_ptr);
				std::cout << stacksize << std::endl;

				Lua::luaL_loadstring(new_lua_State_ptr, "SpawnEvent{ DisplayText = { text = \"THIS TEXT IS CALLED FROM LUA_DOFILE FROM C!\", x = 0, y = 0, duration = 3, fadeTime = 0.5, scale = 0.1 } }") || lua_pcallk(new_lua_State_ptr, 0, -1, 0, 0, NULL);
				stacksize = origTargetFunction(new_lua_State_ptr);
				std::cout << stacksize << std::endl;
				input.clear();
				commandfound = true;
			}

			if (command == "console") 
			{
				if (redirectconsoleoutput) 
				{
					std::cout << "[*] Disabled game print redirection" << std::endl;
				}
				else
				{
					std::cout << "[*] Enabled game print redirection" << std::endl;
				}
				redirectconsoleoutput = !redirectconsoleoutput;
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

void PreGameTick(__int64 game) 
{
	Lua::ExecuteBuffer();
	//std::cout << game << std::endl;
	//Addtext(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
	//Addtext((gamestructbase + 192), "I live in Pre-Game::Update", -0.2, -0.1, 0.05, 0xFF0000FF);
}
void GameTick(__int64 game,float a2) 
{
	gamestructbase = game;
	gamerender = *(int*)(game + 192);
	//std::cout << game << std::endl;
	PreGameTick(game);

	origGameTickFunction(game, a2);
	//Addtext(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
}

int AddTextHook(__int64 a1, const char* text, float x, float y, float size, int color)
{
	//std::cout << a1 << std::endl;
	//std::cout << *(int*)(gamestructbase + 192) << std::endl;
	//std::cout << text << std::endl;
	//std::cout << x << std::endl;
	//std::cout << y << std::endl;
	//std::cout << a5 << std::endl;
	//std::cout << a6 << std::endl;
	//Sleep(1000);
	//Addtext(a1, "CAN YOU PLEASE WORK", -0.2, 0, 0.05, 0xFF0000FF);
	return Addtext(a1, text, x, y, size, color);
}

//int guibenchmark_cycle = 0;
void CustomTextDoRender()
{
	GUIConsole::Draw();
	MenuSystem::GUI::Draw();
	
}
void NetGuiHook(__int64 a1, __int64 a2, float a3)
{
	// add custom text render barn here cuz the game will call it while queueing render is possible
	CustomTextDoRender();
	Addtext(gamerender, "Journey Detour v1.0.1", -1.75, -1.0, 0.05, 0xFFFFFFFF);
	//Addtext(gamerender, " <9002>", -1.5, -0.9, 0.05, 0xFF0000FF);

	
	
	//int result = Addtext(gamestructbase + 192, "TESTTEST", (float)-0.1, (float)-0.1, (float)0.5, 136);
	//std::cout << result << std::endl;
	origNetGuiFunction(a1, a2, a3);
}





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

void PhyrePrintf(unsigned int severity, const char* a2, ...) 
{
	spdlog::info("PhyrePrint: {}", a2);
	va_list args;
	va_start(args, a2);
	vprintf(a2, args);
	va_end(args);
}

const char *__cdecl hooked_debug_print(const char *fmt, ...)
{
	if (!redirectconsoleoutput) 
	{
		return fmt;
	}
	va_list args;
	char buffer[16384];
	va_start(args, fmt);
	vsprintf(buffer,fmt,args);
	va_end(args);
	spdlog::info(buffer);
	GUIConsole::AppendLine(buffer);
	return fmt;
}




int detourLuaState()
{
	using namespace Lua;
	HookEnabler hook;
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x14031AEC0, &_gettop, reinterpret_cast<LPVOID*>(&origTargetFunction));
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x1402E3090, &hooked_debug_print, reinterpret_cast<LPVOID*>(&origTargetdebugprintFunction));
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x140333040, &luaB_print_f, reinterpret_cast<LPVOID*>(&origLuaBPrintFunction));
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x1400DAD40, &GameTick, reinterpret_cast<LPVOID*>(&origGameTickFunction));
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x14010DA00, &NetGuiHook, reinterpret_cast<LPVOID*>(&origNetGuiFunction));
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x14026EAA0, &AddTextHook, reinterpret_cast<LPVOID*>(&Addtext));
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x140381B60, &PhyrePrintf, reinterpret_cast<LPVOID*>(&origPhyrePrintFunction));

	return 0;

}

int refKey = 0;

int WINAPI main()
{
	using namespace Lua;
	ConsoleSetup();

	InitializeLuaGUI();
	// newFunction = (gettop)_gettop;
	// gettop
	baseAddress = (uintptr_t)GetModuleHandle(NULL);


	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	detourLuaState();

	while (luaStateObtained == false)
	{
		Sleep(1000);
	}

	// to juesto: uncomment this if you want to try running code from a new luastate. also change the two function calls using lua_state_ptr to new_ptr.
	new_lua_State_ptr = Lua::CreateThread();
	// luaL_loadfilex_f(new_lua_State_ptr, "main.lua", NULL) || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);

	consoleInputThreadHandle = CreateThread(0, 0, ConsoleInputThread, 0, 0, NULL);

	Lua::AppendBuffer("Vars.Game.cheatsEnabled(true)");

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

		if (GetAsyncKeyState(VK_RETURN) & 1 && isNetGUIEnabled)
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
			if (redirectconsoleoutput)
			{
				std::cout << "[*] Disabled game print redirection" << std::endl;
			}
			else
			{
				std::cout << "[*] Enabled game print redirection" << std::endl;
			}
			redirectconsoleoutput = !redirectconsoleoutput;

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
