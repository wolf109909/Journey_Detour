#pragma once
#include <Windows.h>

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

typedef int(__cdecl* _gettop)(lua_State*);

typedef lua_State* (__cdecl* _newThread)(lua_State*);

typedef void(__cdecl* _pushinteger)(lua_State*, int);

typedef int(__cdecl* _tointeger)(lua_State*, int);

typedef int(__cdecl* _luaStatus)(lua_State*);

typedef int(__cdecl* _luaL_loadstring)(lua_State*, const char*);

typedef const char* (__cdecl* _lua_tostring)(lua_State*, int, size_t*);

typedef int(__cdecl* _lua_getglobal)(lua_State*, const char*);

typedef void(__cdecl* _lua_pushvalue)(lua_State*, int);

typedef float(__cdecl* _drawdebugtext)(uintptr_t, const char*, float, uintptr_t, float, int);

typedef int(__cdecl* _luaL_error)(lua_State*, const char*, ...);

typedef void(__cdecl* _lua_settop)(lua_State*, int);

typedef int (*_lua_debugdostring)(lua_State*, const char*);

typedef int (*_luaL_loadfilex)(lua_State* L, const char* filename, const char* mode);

typedef int (*_lua_pcall)(lua_State* L, int nargs, int nresults, int errfunc);

typedef int (*_lua_KFunction)(lua_State* L, int status, intptr_t ctx);

typedef int (*_close_state)(lua_State* L);

typedef int (*_lua_pcallk)(lua_State* L, int nargs, int nresults, int errfunc, intptr_t ctx, _lua_KFunction k);

typedef void(__cdecl* _lua_callk)(lua_State*, int, int, intptr_t ctx, _lua_KFunction);

typedef int (*_luaL_loadbuffer)(lua_State* L, char* buff, size_t size, char* name);

typedef int (*_lua_pcall)(lua_State* L, int nargs, int nresults, int errfunc);

// typedef const char* lua_tostring_(lua_State* L, int32_t idx);
//typedef uint32_t lua_isstring_(lua_State* L, int32_t idx);

typedef lua_State* _lua_newthread_(lua_State* L);


class LuaManager {
public:
	// Lua initialization stuff
	bool b_Initialized = false;
	lua_State* L;

	// Lua type to names
	_luaL_loadstring luaL_loadstring;
	_lua_pcall lua_pcall;
	_luaL_loadfilex luaL_loadfilex;
	_lua_tostring lua_tostring;
	_lua_getglobal lua_getglobal;
	_lua_pushvalue lua_pushvalue;
	_lua_callk lua_callk;
	_luaL_error luaL_error;
	_lua_settop lua_settop;
	_lua_close lua_close;
	_lua_pcallk lua_pcallk;
	_newThread lua_newthread;
	_close_state close_state;
	_lua_debugdostring lua_debugdostring;
	// Lua hooking functions
	void Initialize();
	// Lua Manager stuff
	std::vector<std::string> m_vLuaExecuteBuffer;
	void AppendBuffer(const std::string& buffer);
	void ExecuteBuffer();
};

extern LuaManager* g_pLua;