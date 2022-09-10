//
// Created by scrci on 2022/9/10.
//

#include "lua.h"

Lua::_luaL_loadstring Lua::luaL_loadstring = (Lua::_luaL_loadstring) Global::Bases::LuaLoadString;
Lua::_lua_pcall Lua::lua_pcall = (Lua::_lua_pcall) Global::Bases::LuaPCall;
Lua::_luaL_loadfilex Lua::luaL_loadfilex = (Lua::_luaL_loadfilex) Global::Bases::LuaLoadFilex;
Lua::lua_tostring_ Lua::lua_tostring = (Lua::lua_tostring_) Global::Bases::LuaToString;
Lua::_lua_getglobal Lua::lua_getglobal = (Lua::_lua_getglobal) Global::Bases::LuaGetGlobal;
Lua::_lua_pushvalue Lua::lua_pushvalue = (Lua::_lua_pushvalue) Global::Bases::LuaPushValue;
Lua::_lua_callk Lua::lua_callk = (Lua::_lua_callk) Global::Bases::LuaCallK;
Lua::_luaL_error Lua::luaL_error = (Lua::_luaL_error) Global::Bases::LuaLError;
Lua::_lua_settop Lua::lua_settop = (Lua::_lua_settop) Global::Bases::LuaSetTop;
Lua::_lua_close Lua::lua_close = (Lua::_lua_close) Global::Bases::LuaClose;
Lua::_lua_pcallk Lua::lua_pcallk = (Lua::_lua_pcallk) Global::Bases::LuaPCallK;
Lua::newThread Lua::lua_newthread = (Lua::newThread) Global::Bases::LuaNewThread;
//uintptr_t Lua::luab_print_ptr = 0x140333040;
Lua::_close_state Lua::close_state = (Lua::_close_state) Global::Bases::LuaCloseState;
Lua::_lua_debugdostring Lua::lua_debugdostring = (Lua::_lua_debugdostring) Global::Bases::LuaDebugDoString;

std::vector<std::string> Lua::m_LuaExecuteBuffer;
Lua::lua_State *Lua::lua_State_ptr = 0;
Lua::lua_State *Lua::new_lua_State_ptr = 0;
Lua::_debug_print Lua::DebugPrint = NULL;
Lua::gettop Lua::GetTop = NULL;
Lua::_luaB_print Lua::LuaB_print = NULL;
bool Lua::luaStateObtained = false;
int Lua::luastatus;

int GetTopHook(Lua::lua_State *L) {
    // std::cout << "[!] Hook called :O" << std::endl;
    if (Lua::lua_State_ptr == nullptr) {
        Lua::lua_State_ptr = (Lua::lua_State *) L;
        // std::cout << "[+] State Obtained: \t" << std::hex << lua_State_ptr << std::endl;
        spdlog::info("Successfully obtained state:{}", *Lua::lua_State_ptr);
        if (MH_DisableHook((LPVOID) Global::Bases::GetTop) != MH_OK) {
            std::cout << "[+] Hook disabled" << std::endl;
        }
    }
    Lua::luaStateObtained = true;

    // return (*(DWORD*)(state + 0x18) - *(DWORD*)(state + 0x10)) >> 3;
    return Lua::GetTop(L);
}

int LuaB_printHook(Lua::lua_State *L) {
    int n = Lua::GetTop(L); /* number of arguments */
    int i;
    Lua::lua_getglobal(L, "tostring");
    for (i = 1; i <= n; i++) {
        const char *s;
        size_t l;
        Lua::lua_pushvalue(L, -1); /* function to be called */
        Lua::lua_pushvalue(L, i);    /* value to print */
        Lua::lua_call(L, 1, 1);
        s = Lua::lua_tostring(L, -1, &l); /* get result */
        if (s == nullptr)
            return Lua::luaL_error(L, "'tostring' must return a string to 'print'");
        if (i > 1)
            lua_writestring("\t", 1);
        lua_writestring(s, l);
        Lua::lua_pop(L, 1); /* pop result */
    }
    lua_writeline();
    // LuaB_print(new_lua_State_ptr);
    return 0;
}

const char *__cdecl DebugPrintHook(const char *fmt, ...) {
    if (!Global::console_OutputRedirect) {
        return fmt;
    }
    va_list args;
    char buffer[16384];
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    spdlog::info(buffer);
    GUIConsole::AppendLine(buffer);
    return fmt;
}

Lua::lua_State *Lua::CreateThread() {
    spdlog::info("Creating new thread");
    // std::cout << "[+] Calling newThread" << std::endl;
    lua_State *thread = lua_newthread(lua_State_ptr);
    // std::cout << "[+] New LuaState: " << thread << std::endl;
    // lua_pop(lua_State_ptr, 1);
    return thread;
}

void Lua::AppendBuffer(const std::string& buffer) {
    //std::cout << buffer << std::endl;
    m_LuaExecuteBuffer.push_back(buffer);
}

void Lua::ExecuteBuffer() {
    if (m_LuaExecuteBuffer.empty())
        return;
    if (Game::LuaState == 0)
        return;
    for (const auto& buffer: m_LuaExecuteBuffer) {
        luaL_loadstring((lua_State*)Game::LuaState, buffer.c_str());
        if (lua_pcallk((lua_State*)Game::LuaState, 0, 0, 0, 0, NULL) != 0) {
            std::cout << "ERROR: " << lua_tostring((lua_State*)Game::LuaState, -1, NULL) << std::endl;
        }
        //std::cout << "buffer executed" << std::endl;
        m_LuaExecuteBuffer.erase(m_LuaExecuteBuffer.begin());
    }

}

void Lua::Initialize(HookEnabler hook) {
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Global::Bases::GetTop, &GetTopHook, reinterpret_cast<LPVOID *>(&Lua::GetTop));
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Global::Bases::DebugPrint, &DebugPrintHook, reinterpret_cast<LPVOID *>(&Lua::DebugPrint));
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Global::Bases::LuaB_print, &LuaB_printHook, reinterpret_cast<LPVOID *>(&Lua::LuaB_print));
}