#include "pch.h"
#include "lua.h"
#include "memory.h"
AUTOHOOK_INIT()

LuaManager* g_pLua;

AUTOHOOK_ABSOLUTEADDR(lua_gettop, (LPVOID)0x14031AEC0,
    int, __fastcall, (lua_State* _L))
{
    if(g_pLua->b_Initialized)
        return lua_gettop(_L);

    g_pLua->L = _L;
    if (MH_DisableHook((LPVOID)0x14031AEC0) != MH_OK) {
       spdlog::info("[LuaManager] Failed to disable hook");
    }
    g_pLua->b_Initialized = true;
    spdlog::info("[LuaManager] Successfully obtained lua state");
    return lua_gettop(_L);
}

void LuaManager::Initialize()
{
    spdlog::info("[LuaManager] Initializing...");
    g_pLua = new LuaManager;
    g_pLua->luaL_loadstring = MemoryAddress(0x14031E250).As<_luaL_loadstring>();
    g_pLua->lua_pcall = MemoryAddress(0x1403278C0).As<_lua_pcall>();
    g_pLua->luaL_loadfilex = MemoryAddress(0x14031DEA0).As<_luaL_loadfilex>();
    g_pLua->lua_tostring = MemoryAddress(0x14031B3B0).As<_lua_tostring>();
    g_pLua->lua_getglobal = MemoryAddress(0x14031BA50).As<_lua_getglobal>();
    g_pLua->lua_pushvalue = MemoryAddress(0x14031AF30).As<_lua_pushvalue>();
    g_pLua->lua_callk = MemoryAddress(0x14031C340).As<_lua_callk>();
    g_pLua->luaL_error = MemoryAddress(0x14031DAA0).As<_luaL_error>();
    g_pLua->lua_settop = MemoryAddress(0x14031AEE0).As<_lua_settop>();
    g_pLua->lua_close = MemoryAddress(0x14031F6E0).As<_lua_close>();
    g_pLua->lua_pcallk = MemoryAddress(0x14031C3D0).As<_lua_pcallk>();
    g_pLua->lua_newthread = MemoryAddress(0x14031F700).As<_newThread>();
    g_pLua->close_state = MemoryAddress(0x14031FB30).As<_close_state>();
    g_pLua->lua_debugdostring = MemoryAddress(0x1402D7460).As<_lua_debugdostring>();
    AUTOHOOK_DISPATCH()
}

void LuaManager::AppendBuffer(const std::string& buffer) {
    //std::cout << buffer << std::endl;
    m_vLuaExecuteBuffer.push_back(buffer);
}
void LuaManager::ExecuteBuffer() {
    if (!b_Initialized)
        return;
    if (m_vLuaExecuteBuffer.empty())
        return;
    for (const auto& buffer : m_vLuaExecuteBuffer) {
        luaL_loadstring(L, buffer.c_str());
        if (lua_pcallk(L, 0, 0, 0, 0, NULL) != 0) {
            spdlog::error("[LuaManager] Script Execute Error: {}",lua_tostring(L, -1, NULL));
            //std::cout << "ERROR: " << lua_tostring(L, -1, NULL) << std::endl;
        }
        //std::cout << "buffer executed" << std::endl;
        m_vLuaExecuteBuffer.erase(m_vLuaExecuteBuffer.begin());
    }

}