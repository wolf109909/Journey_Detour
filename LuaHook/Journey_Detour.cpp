// LuaInjector.cpp : Defines the exported functions for the DLL application.
//
// Standard imports
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
//#pragma comment(lib, "lua53.lib")
#if defined _M_X64
#pragma comment(lib, "libMinHook-x64-v141-mdd.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mdd.lib")
#endif

#define lua_writestring(s,l)   fwrite((s), sizeof(char), (l), stdout)
#define lua_writeline()        (lua_writestring("\n", 1), fflush(stdout))
#define LUA_KCONTEXT	intptr_t
#define lua_call(L,n,r)		lua_callk_f(L, (n), (r), 0, NULL)
#define lua_pop(L,n)		lua_settop_f(L, -(n)-1)

bool luaStateObtained = false;


typedef void* lua_State;
// LUA STATE HOOK DEFINITIONS
uint64_t baseAddress = 0;
uintptr_t targetFunction = 0;
uintptr_t printf_addr = 0;
uintptr_t newthreadptr = 0;


lua_State* lua_State_ptr = 0;
lua_State* new_lua_State_ptr = 0;
typedef intptr_t lua_KContext;
typedef void(__cdecl*_debug_print)(const char*);
typedef int (__cdecl* _luaB_print)(lua_State*);
typedef int(__cdecl* gettop)(lua_State*);
typedef lua_State* (__cdecl* newThread)(lua_State*);
typedef void(__cdecl* pushinteger)(lua_State*, int);
typedef int(__cdecl* tointeger)(lua_State*, int);
typedef int(__cdecl* _luaStatus)(lua_State*);
typedef int(__cdecl* _luaL_loadstring)(lua_State*, const char*);
typedef const char*(__cdecl* lua_tostring_)(lua_State*, int, size_t*);
typedef int(__cdecl* _lua_getglobal)(lua_State*, const char*);
typedef void(__cdecl* _lua_pushvalue)(lua_State*, int);
typedef float(__cdecl* _drawdebugtext)(uintptr_t,const char*, float, uintptr_t, float, int);
typedef int(__cdecl* _luaL_error)(lua_State* , const char* , ...);
typedef void(__cdecl* _lua_settop)(lua_State* , int );
//typedef int(__cdecl *_lua_pcall)(lua_State *, int, int, int);

uintptr_t luab_print_ptr = 0x140333040;
//uintptr_t lua_getglobal_ptr = 0x14031BE70;


_luaB_print origLuaBPrintFunction = NULL;



_debug_print origTargetdebugprintFunction = NULL;
gettop origTargetFunction = NULL;
typedef int(*_lua_debugdostring)(lua_State*, const char*);
typedef int(*_luaL_loadfilex)(lua_State* L, const char* filename, const char* mode);
typedef int(*_lua_pcall)(lua_State* L, int nargs, int nresults, int errfunc);
typedef int(*lua_KFunction)(lua_State* L, int status, intptr_t ctx);
typedef int (*_close_state)(lua_State* L);
typedef int (*_lua_pcallk)(lua_State *L, int nargs, int nresults, int errfunc, intptr_t ctx, lua_KFunction k);
typedef void(__cdecl* _lua_callk)(lua_State*, int, int, intptr_t ctx, lua_KFunction);
typedef int luaL_loadbuffer_(lua_State* L, char* buff, size_t size, char* name);
typedef int lua_pcall_(lua_State* L, int nargs, int nresults, int errfunc);
//typedef const char* lua_tostring_(lua_State* L, int32_t idx);
typedef uint32_t lua_isstring_(lua_State* L, int32_t idx);
typedef lua_State* lua_newthread_(lua_State* L);
typedef void logPointer(std::string name, uint64_t pointer);

//_debug_print debug_print = (_debug_print)0x0;


_lua_pcallk lua_pcallk_f = (_lua_pcallk)0x14031C3D0;
_luaL_loadstring luaL_loadstring_f = (_luaL_loadstring)0x14031E250;
_lua_pcall lua_pcall_f = (_lua_pcall)0x1403278C0;
_luaL_loadfilex luaL_loadfilex_f = (_luaL_loadfilex)0x14031DEA0;
lua_tostring_ lua_tostring_f = (lua_tostring_)0x14031B3B0;
_lua_getglobal lua_getglobal_f = (_lua_getglobal)0x14031BA50;
_lua_pushvalue lua_pushvalue_f = (_lua_pushvalue)0x14031AF30;
_lua_callk lua_callk_f = (_lua_callk)0x14031C340;
_luaL_error luaL_error_f = (_luaL_error)0x14031DAA0;
_lua_settop lua_settop_f = (_lua_settop)0x14031AEE0;
_drawdebugtext drawdebugtext_f = (_drawdebugtext)0x14026EAA0;
//DWORD newthreadptr = (uintptr_t)GetModuleHandle(NULL) + 0x31EB00;

_close_state close_state = (_close_state)0x14031FB30;
_lua_debugdostring lua_debugdostring = (_lua_debugdostring)0x1402D7460;
typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI* NtResumeProcess)(IN HANDLE ProcessHandle);
int luastatus;
bool isNetGUIEnabled = false;
bool commandfound = false;
HANDLE consoleInputThreadHandle = NULL;
//const char* Console_luaScriptCommandName = "script";


void InitializeLuaGUI() 
{
	const char* LGUI_DRAW_BOX = "SpawnEvent{HudAddElement = {alpha = 0.33, dimx = 1.3, dimy = .85, duration = .6, elementName = \"Black\", fadeIn = 0, fadeOut = 0.25, posx = 0.13, posy = -0.25, texName = \"Black\", depth = 0, isPauseElem = true}}";


}


DWORD WINAPI ConsoleInputThread(PVOID pThreadParameter)
{
	
	FILE* fp = nullptr;
	freopen_s(&fp, "CONIN$", "r", stdin);

	std::cout << "Ready to receive console commands." << std::endl;	

	{
		// Process console input
		std::string input;
		while (std::getline(std::cin, input))
		{
			input += "\n";
			//std::cout << input << std::endl;
			
			const char* command = strtok((char*)input.c_str(), " ");
			std::string strcommand = command;
			
			//std::cout << "command:" << command << std::endl;
			if (!strcmp(command,"script"))
			{
				try {

					input = input.substr(7);

				}

				catch (std::out_of_range&) {

					std::cout << "script error: no input given!" << std::endl;
					
				}

				//std::cout << "codestring:" << input << std::endl;
				luaL_loadstring_f(new_lua_State_ptr, input.c_str());
				luastatus = lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
				if (luastatus != 0) 
				{
					printf("%s\n", lua_tostring_f(new_lua_State_ptr, -1, NULL));
				}
				std::cout << "execute status:" << luastatus << std::endl;
				input.clear();
				commandfound = true;
				lua_settop_f(new_lua_State_ptr, 0);
			}
			
			
			if(!strcmp(command, "queuelevel"))
			{
				try {

					input = input.substr(11);

				}

				catch (std::out_of_range&) {

					std::cout << "queuelevel error: no input given!" << std::endl;

				}

				input = "game:QueueLevel(\"" + input.substr(0, input.size() - 1) + "\")";

				std::cout << input << std::endl;
				luaL_loadstring_f(new_lua_State_ptr, input.c_str());
				luastatus = lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
				std::cout << "execute status:" << luastatus << std::endl;
				input.clear();
				commandfound = true;
			}
			
			if (!strcmp(strcommand.substr(0, strcommand.size() - 1).c_str(), "netgui"))
			{
				
				luaL_loadstring_f(new_lua_State_ptr, "game:netGui():ToggleEnabled()") || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
				isNetGUIEnabled = !isNetGUIEnabled;
				input.clear();
				commandfound = true;
			}
			

			if (!strcmp(strcommand.substr(0, strcommand.size() - 1).c_str(), "test"))
			{
				std::cout << "Test function called!" << std::endl;
				//uintptr_t debugtexta1 = 0x1400D3940;
				//uintptr_t debugtexty = 0x1400A2E90;
				//int debugtexta5 = 0xFFFFFF;
				
				//	drawdebugtext_f(debugtexta1, "Debug Print Test!!!",1, debugtexty ,1, debugtexta5);


				int stacksize = origTargetFunction(new_lua_State_ptr);
				std::cout << stacksize << std::endl;
				
				luaL_loadstring_f(new_lua_State_ptr,"SpawnEvent{ DisplayText = { text = \"THIS TEXT IS CALLED FROM LUA_DOFILE FROM C!\", x = 0, y = 0, duration = 3, fadeTime = 0.5, scale = 0.1 } }") || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
				stacksize = origTargetFunction(new_lua_State_ptr);
				std::cout << stacksize << std::endl;
				input.clear();
				commandfound = true;
			}

			
			
			if (commandfound == false) 
			{
				std::cout << "command not found!" << std::endl;
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
	//freopen("CONIN$", "r", stdin);

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
	

	std::cout << "Injecting..." << std::endl;
	//std::cout << baseAddress << std::endl; 
	//std::cout << newthreadptr << std::endl;
}


gettop newFunction = NULL;



int luaB_print_f(lua_State* L) {
	int n = origTargetFunction(L);  /* number of arguments */
	int i;
	lua_getglobal_f(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char* s;
		size_t l;
		lua_pushvalue_f(L, -1);  /* function to be called */
		lua_pushvalue_f(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring_f(L, -1, &l);  /* get result */
		if (s == NULL)
			return luaL_error_f(L, "'tostring' must return a string to 'print'");
		if (i > 1) lua_writestring("\t", 1);
		lua_writestring(s, l);
		lua_pop(L, 1);  /* pop result */
	}
	lua_writeline();
	//origLuaBPrintFunction(new_lua_State_ptr);
	return 0;
}


int _gettop(lua_State* L)
{
	//std::cout << "[!] Hook called :O" << std::endl;
	if (lua_State_ptr == 0)
	{
		lua_State_ptr = (lua_State*)L;
		//std::cout << "[+] State Obtained: \t" << std::hex << lua_State_ptr << std::endl;

		if (MH_DisableHook((LPVOID)targetFunction) != MH_OK)
		{
			std::cout << "[+] Hook disabled" << std::endl;
		}
	}
	luaStateObtained = true;
	;
	//return (*(DWORD*)(state + 0x18) - *(DWORD*)(state + 0x10)) >> 3;
	return origTargetFunction(L);
}



//Get all module related info, this will include the base DLL.
//and the size of the module
MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

//Pattern Scan https://guidedhacking.com/threads/c-signature-scan-pattern-scanning-tutorial-difficulty-3-10.3981
uintptr_t FindPattern(char* module, char* pattern, char* mask)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo(module);

	//Assign our base and module size
	uintptr_t base = (uintptr_t)mInfo.lpBaseOfDll;

	uintptr_t size = (uintptr_t)mInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default,
			//or if the bytes match then we keep searching until finding it or not
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		if (found)
		{
			std::cout << "[+] Pattern found: " << base + i << std::endl;
			return base + i;
		}
	}
	std::cout << "[-] Pattern not found" << std::endl;
	return NULL;
}



void printValues()
{
	std::cout << "targetFunction Addr: \t" << std::hex << targetFunction << std::endl;
	std::cout << "newFunction Addr: \t" << std::hex << &_gettop << std::endl;
	//std::cout << "fn luaL_loadstring_p: \t" << std::hex << luaL_loadstring_p << std::endl;
	
	std::cout << "fn lua_newthread_p: \t" << std::hex << newthreadptr << std::endl;
	//std::cout << "fn lua_pushinteger_p: \t" << std::hex << val.lua_pushinteger_p << std::endl;
	//std::cout << "fn lua_tointeger_p: \t" << std::hex << val.lua_tointeger_p << std::endl;
	//std::cout << "fn lua_status_p: \t" << std::hex << val.lua_status_p << std::endl;

	//std::cout << "fn lua_pcall_p: \t" << std::hex << val.lua_pcall_p << std::endl;
	
}

const char* __cdecl hooked_debug_print(const char* fmt ,...)
{
	
	va_list args;
	const char* buf;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	return fmt;
}

int detourLuaState()
{
	//_gettop(lua_State);//i don't think you need this but if you do you can just add it!
	std::cout << "[+] Hooking targetFunction: " << (LPVOID)targetFunction << std::endl;


	if (MH_CreateHook((LPVOID)targetFunction, &_gettop, reinterpret_cast<LPVOID*>(&origTargetFunction)) != MH_OK)
	{
		std::cout << "[-] Hooking failed" << std::endl;
		return 1;
	}
	std::cout << "[+] Hooked" << std::endl;
	if (MH_EnableHook((LPVOID)targetFunction) != MH_OK)
	{
		return 1;
	}
	std::cout << "[+] Hooked enabled" << std::endl;
	std::cout << "[+] origTargetFunction " << std::hex << origTargetFunction << std::endl;

	if (MH_CreateHook((LPVOID)printf_addr, &hooked_debug_print, reinterpret_cast<LPVOID*>(&origTargetdebugprintFunction)) != MH_OK)
	{
		std::cout << "[-] Hooking prinf failed" << std::endl;
		return 1;
	}
	std::cout << "[+] DEBUG PRINT Hooked" << std::endl;
	if (MH_EnableHook((LPVOID)printf_addr) != MH_OK)
	{
		return 1;
	}
	std::cout << "[+] Hooked enabled" << std::endl;
	std::cout << "[+] printf_addr " << std::hex << printf_addr << std::endl;


	//48 89 5C 24 08 57 48 83  EC 20 48 8B 41

	if (MH_CreateHook((LPVOID)luab_print_ptr, &luaB_print_f, reinterpret_cast<LPVOID*>(&origLuaBPrintFunction)) != MH_OK)
	{
		std::cout << "[-] Hooking failed" << std::endl;
		return 1;
	}
	std::cout << "[+] Hooked" << std::endl;
	if (MH_EnableHook((LPVOID)luab_print_ptr) != MH_OK)
	{
		return 1;
	}
	std::cout << "[+] Hooked enabled" << std::endl;
	std::cout << "[+] origTargetFunction " << std::hex << origLuaBPrintFunction << std::endl;



	/*
	if (MH_CreateHook((LPVOID)newthread, &hooked_debug_print, reinterpret_cast<LPVOID*>(&origTargetdebugprintFunction)) != MH_OK)
	{
		std::cout << "[-] Hooking newthread failed" << std::endl;
		return 1;
	}
	std::cout << "[+] newthread Hooked" << std::endl;
	if (MH_EnableHook((LPVOID)newthread_addr) != MH_OK)
	{
		return 1;
	}
	std::cout << "[+] Hooked enabled" << std::endl;
	std::cout << "[+] newthread_addr " << std::hex << printf_addr << std::endl;
	*/
	return 0;
	
	//DetourTransactionBegin();
	//DetourUpdateThread(GetCurrentThread());
	//DetourAttach(&(LPVOID&)val.lua_gettop_p, (PBYTE)_gettop); // Detours the original lua_gettop_p with our _gettop
	//DetourTransactionCommit();
	
}


int refKey = 0;




lua_State* luaCreateThread()
{
	newThread lua_newthread_f = (newThread)newthreadptr;
	//newThread lua_newthread_f = (newThread)newthreadptr;
	std::cout << "[+] Calling newThread" << std::endl;
	lua_State* thread = (lua_newthread_f)(lua_State_ptr);
	std::cout << "[+] New LuaState: " << thread << std::endl;

	return thread;
}

int WINAPI main()
{
	ConsoleSetup();
	InitializeLuaGUI();
	//newFunction = (gettop)_gettop;
	//gettop
	baseAddress = (uintptr_t)GetModuleHandle(NULL);
	char* test = new char[14];
	char* test2 = new char[14];
	//strcpy(test, "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x30\x45\x33\xC9");
	strcpy(test, "\x48\x8B\x41\x20\x48\x8B\x51\x10\x48\x2B\x10\x48\x8D");
	strcpy(test2, "xxxxxxxxxxxxx");
	targetFunction = FindPattern(NULL, test, test2);

	//some print func
	char* debug_print_hex = new char[25];
	char* debug_print_mask = new char[25];
	strcpy(debug_print_hex, "\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x57\xB8\x48");
	strcpy(debug_print_mask, "xxxxxxxxxxxxxxxxxxxxxxxx");
	printf_addr = FindPattern(NULL, debug_print_hex, debug_print_mask);
	
	//newthread
	char* newthreadhex = new char[25];
	char* newthreadmask = new char[25];
	strcpy(newthreadhex, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x71\x18\x48\x8B\xF9\x48\x83");
	strcpy(newthreadmask, "xxxxxxxxxxxxxxxxxxxxxxxx");
	newthreadptr = FindPattern(NULL, newthreadhex, newthreadmask);
	
	
	
	//newThread lua_newthread_f = (newThread)newthread_addr;

	
	
	printValues();

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
	new_lua_State_ptr = luaCreateThread();
	//luaL_loadfilex_f(new_lua_State_ptr, "main.lua", NULL) || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
	
	
	consoleInputThreadHandle = CreateThread(0, 0, ConsoleInputThread, 0, 0, NULL);
	
	luaL_loadstring_f(new_lua_State_ptr, "Vars.Game.cheatsEnabled(true)") || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
	
	while (true)
	{
		int status;
		if (GetAsyncKeyState(VK_RETURN) & 1 && isNetGUIEnabled)
		{
			luaL_loadstring_f(new_lua_State_ptr, "game:netGui():ExecuteSelectedItem(game)") || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
			

		}
		if (GetAsyncKeyState(VK_F9) & 1)
		{
			luaL_loadfilex_f(new_lua_State_ptr, "main.lua", NULL) || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0,0,NULL);
			std::cout << "[!] Main.lua Executed" << std::endl;


			//std::cout << "[+] LuaStatus: " << val.lua_status_p(lua_State_ptr) << std::endl;
			//std::cout << "[+] New LuaStatus: " << val.lua_status_p(new_lua_State_ptr) << std::endl;
		}
		if (GetAsyncKeyState(VK_F8) & 1)
		{
			luaL_loadstring_f(new_lua_State_ptr,"dofile('RunOnce.lua')") || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
			std::cout << "[!] lua_loadstring called!" << std::endl;


			//std::cout << "[+] LuaStatus: " << val.lua_status_p(lua_State_ptr) << std::endl;
			//std::cout << "[+] New LuaStatus: " << val.lua_status_p(new_lua_State_ptr) << std::endl;
		}
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			luaL_loadstring_f(new_lua_State_ptr, "dofile('main.lua')") || lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
			std::cout << "[!] lua_loadstring called!" << std::endl;
			

			//std::cout << "[+] LuaStatus: " << val.lua_status_p(lua_State_ptr) << std::endl;
			//std::cout << "[+] New LuaStatus: " << val.lua_status_p(new_lua_State_ptr) << std::endl;
		}
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			char luacodestring[128] = { 0 };
			std::cout << "please input lua string:" << std::endl;
			std::cin.getline(luacodestring,128);
			luaL_loadstring_f(new_lua_State_ptr, luacodestring);
			std::cout << "codestring:" << luacodestring << std::endl;
			status = lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
			std::cout << "execute status:" << status << std::endl;
			//luacodestring = 0;

		}
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			luaL_loadstring_f(new_lua_State_ptr, "game:debugHud():CycleMode()");
			status = lua_pcallk_f(new_lua_State_ptr, 0, -1, 0, 0, NULL);
			std::cout << status << std::endl;
			std::cout << "[!] Switching debugHud mode." << std::endl;
		}
		Sleep(100);
	}
	return 1;

}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
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

