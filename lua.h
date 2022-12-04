
#pragma once


#include "sharedlibrary.h"
#include "hookutils.h"
#include "console.h"

namespace Lua {
#define lua_writestring(s, l) fwrite((s), sizeof(char), (l), stdout)
#define lua_writeline() (lua_writestring("\n", 1), fflush(stdout))
#define LUA_KCONTEXT intptr_t
#define lua_call(L, n, r) lua_callk(L, (n), (r), 0, NULL)
#define lua_pop(L, n) lua_settop(L, -(n)-1)

    typedef intptr_t lua_KContext;
    typedef void *lua_State;

    typedef void(__cdecl *_lua_close)(lua_State *L);

    typedef void(__cdecl *_debug_print)(const char *a1, ...);

    typedef int(__cdecl *_luaB_print)(lua_State *);

    typedef int(__cdecl *gettop)(lua_State *);

    typedef lua_State *(__cdecl *newThread)(lua_State *);

    typedef void(__cdecl *pushinteger)(lua_State *, int);

    typedef int(__cdecl *tointeger)(lua_State *, int);

    typedef int(__cdecl *_luaStatus)(lua_State *);

    typedef int(__cdecl *_luaL_loadstring)(lua_State *, const char *);

    typedef const char *(__cdecl *lua_tostring_)(lua_State *, int, size_t *);

    typedef int(__cdecl *_lua_getglobal)(lua_State *, const char *);

    typedef void(__cdecl *_lua_pushvalue)(lua_State *, int);

    typedef float(__cdecl *_drawdebugtext)(uintptr_t, const char *, float, uintptr_t, float, int);

    typedef int(__cdecl *_luaL_error)(lua_State *, const char *, ...);

    typedef void(__cdecl *_lua_settop)(lua_State *, int);

    typedef int (*_lua_debugdostring)(lua_State *, const char *);

    typedef int (*_luaL_loadfilex)(lua_State *L, const char *filename, const char *mode);

    typedef int (*_lua_pcall)(lua_State *L, int nargs, int nresults, int errfunc);

    typedef int (*lua_KFunction)(lua_State *L, int status, intptr_t ctx);

    typedef int (*_close_state)(lua_State *L);

    typedef int (*_lua_pcallk)(lua_State *L, int nargs, int nresults, int errfunc, intptr_t ctx, lua_KFunction k);

    typedef void(__cdecl *_lua_callk)(lua_State *, int, int, intptr_t ctx, lua_KFunction);

    typedef int luaL_loadbuffer_(lua_State *L, char *buff, size_t size, char *name);

    typedef int lua_pcall_(lua_State *L, int nargs, int nresults, int errfunc);

    // typedef const char* lua_tostring_(lua_State* L, int32_t idx);
    typedef uint32_t lua_isstring_(lua_State *L, int32_t idx);

    typedef lua_State *lua_newthread_(lua_State *L);

    extern _luaL_loadstring             luaL_loadstring;
    extern lua_tostring_                lua_tostring;
    extern _lua_getglobal               lua_getglobal;
    extern _lua_pushvalue               lua_pushvalue;
    extern _lua_callk                   lua_callk;
    extern _luaL_error                  luaL_error;
    extern _lua_settop                  lua_settop;
    extern _lua_pcallk                  lua_pcallk;
    extern newThread                    lua_newthread;

    extern std::vector<std::string>     m_LuaExecuteBuffer;
    extern lua_State                    *lua_State_ptr;
    extern lua_State                    *new_lua_State_ptr;
    extern _debug_print                 DebugPrint;
    extern gettop                       GetTop;
    extern _luaB_print                  LuaB_print;

    extern bool                         luaStateObtained;
    extern int                          luastatus;
    extern _lua_pcall                   lua_pcall;
    extern _luaL_loadfilex              luaL_loadfilex;
    extern _lua_close                   lua_close;
    extern _close_state                 close_state;
    extern _lua_debugdostring           lua_debugdostring;
    //    extern uintptr_t                    luab_print_ptr;

//    int _gettop(lua_State *L);

//    int LuaB_printHook(lua_State *L);

    lua_State *CreateThread();

    void AppendBuffer(const std::string& buffer);

    void ExecuteBuffer();

    void Initialize(HookEnabler hook);
};

