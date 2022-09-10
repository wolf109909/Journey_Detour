//
// Created by scrci on 2022/9/10.
//

#include "lua.h"

    Lua::_luaL_loadstring Lua::luaL_loadstring = (_luaL_loadstring)0x14031E250;
    Lua::_lua_pcall Lua::lua_pcall = (_lua_pcall)0x1403278C0;
    Lua::_luaL_loadfilex Lua::luaL_loadfilex = (_luaL_loadfilex)0x14031DEA0;
    Lua::lua_tostring_ Lua::lua_tostring = (lua_tostring_)0x14031B3B0;
    Lua::_lua_getglobal Lua::lua_getglobal = (_lua_getglobal)0x14031BA50;
    Lua::_lua_pushvalue Lua::lua_pushvalue = (_lua_pushvalue)0x14031AF30;
    Lua::_lua_callk Lua::lua_callk = (_lua_callk)0x14031C340;
    Lua::_luaL_error Lua::luaL_error = (_luaL_error)0x14031DAA0;
    Lua::_lua_settop Lua::lua_settop = (_lua_settop)0x14031AEE0;
    Lua::_lua_close Lua::lua_close = (_lua_close)0x14031F6E0;
    Lua::_lua_pcallk Lua::lua_pcallk = (_lua_pcallk)0x14031C3D0;
    Lua::newThread Lua::lua_newthread = (newThread)0x14031F700;
    uintptr_t Lua::luab_print_ptr = 0x140333040;
    Lua::_close_state Lua::close_state = (_close_state)0x14031FB30;
    Lua::_lua_debugdostring Lua::lua_debugdostring = (_lua_debugdostring)0x1402D7460;

    std::vector<std::string> Lua::m_LuaExecuteBuffer;
    Lua::lua_State* Lua::lua_State_ptr = 0;
    Lua::lua_State* Lua::new_lua_State_ptr = 0;
    Lua::_debug_print Lua::origTargetdebugprintFunction = NULL;
    Lua::gettop Lua::origTargetFunction = NULL;
    Lua::_luaB_print Lua::origLuaBPrintFunction = NULL;
    bool Lua::luaStateObtained = false;
    int luastatus;
int Lua::_gettop(lua_State *L) {
    // std::cout << "[!] Hook called :O" << std::endl;
    if (lua_State_ptr == 0) {
        lua_State_ptr = (lua_State *) L;
        // std::cout << "[+] State Obtained: \t" << std::hex << lua_State_ptr << std::endl;
        spdlog::info("Successfully obtained state:{}", *lua_State_ptr);
        if (MH_DisableHook((LPVOID) 0x14031AEC0) != MH_OK) {
            std::cout << "[+] Hook disabled" << std::endl;
        }
    }
    luaStateObtained = true;

    // return (*(DWORD*)(state + 0x18) - *(DWORD*)(state + 0x10)) >> 3;
    return origTargetFunction(L);
}

int Lua::luaB_print_f(lua_State *L) {
    int n = origTargetFunction(L); /* number of arguments */
    int i;
    lua_getglobal(L, "tostring");
    for (i = 1; i <= n; i++) {
        const char *s;
        size_t l;
        lua_pushvalue(L, -1); /* function to be called */
        lua_pushvalue(L, i);    /* value to print */
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

Lua::lua_State *Lua::CreateThread() {
    spdlog::info("Creating new thread");
    // std::cout << "[+] Calling newThread" << std::endl;
    lua_State *thread = lua_newthread(lua_State_ptr);
    // std::cout << "[+] New LuaState: " << thread << std::endl;
    // lua_pop(lua_State_ptr, 1);
    return thread;
}

void Lua::AppendBuffer(std::string buffer) {
    //std::cout << buffer << std::endl;
    m_LuaExecuteBuffer.push_back(buffer);
}

void Lua::ExecuteBuffer() {
    if (m_LuaExecuteBuffer.size() == 0)
        return;

    for (auto buffer: m_LuaExecuteBuffer) {
        luaL_loadstring(new_lua_State_ptr, buffer.c_str());
        if (lua_pcallk(new_lua_State_ptr, 0, 0, 0, 0, NULL) != 0) {
            std::cout << "ERROR: " << lua_tostring(new_lua_State_ptr, -1, NULL) << std::endl;
        }
        //std::cout << "buffer executed" << std::endl;
        m_LuaExecuteBuffer.erase(m_LuaExecuteBuffer.begin());
    }

}