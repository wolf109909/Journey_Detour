#pragma once
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
#include "hookutils.h"
#include "spdlog/spdlog.h"
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
	_lua_pcallk lua_pcallk = (_lua_pcallk)0x14031C3D0;
	newThread lua_newthread = (newThread)0x14031F700;
	uintptr_t luab_print_ptr = 0x140333040;
	_close_state close_state = (_close_state)0x14031FB30;
	_lua_debugdostring lua_debugdostring = (_lua_debugdostring)0x1402D7460;

	std::vector<std::string> m_LuaExecuteBuffer;
	lua_State* lua_State_ptr = 0;
	lua_State* new_lua_State_ptr = 0;
	gettop origTargetFunction = NULL;
	_luaB_print origLuaBPrintFunction = NULL;
	bool luaStateObtained = false;
	int luastatus;

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
	void Initialize()
	{
		HookEnabler hook;
		ENABLER_CREATEHOOK(
			hook, (LPVOID)0x140333040, &luaB_print_f, reinterpret_cast<LPVOID*>(&origLuaBPrintFunction));
	}
};