#include "stdafx.h"
#include "luahooks.h"
#include "game.h"
#include "hookutils.h"

Lua* g_Lua = new(Lua);

int Lua::luaB_print_f(lua_State* L)
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

lua_State* Lua::LuaCreateThread()
{
	spdlog::info("Creating new thread");
	// std::cout << "[+] Calling newThread" << std::endl;
	lua_State* thread = lua_newthread(lua_State_ptr);
	// std::cout << "[+] New LuaState: " << thread << std::endl;
	// lua_pop(lua_State_ptr, 1);
	return thread;
}
void Lua::AppendBuffer(std::string buffer)
{
	//std::cout << buffer << std::endl;
	m_LuaExecuteBuffer.push_back(buffer);
}

void Lua::ExecuteBuffer()
{
	if (m_LuaExecuteBuffer.size() == 0)
		return;

	for (auto buffer : m_LuaExecuteBuffer)
	{
		luaL_loadstring(Game::LuaState, buffer.c_str());
		if (lua_pcallk(Game::LuaState, 0, 0, 0, 0, NULL) != 0)
		{
			std::cout << "ERROR: " << lua_tostring(Game::LuaState, -1, NULL) << std::endl;
		}
		//std::cout << "buffer executed" << std::endl;
		m_LuaExecuteBuffer.erase(m_LuaExecuteBuffer.begin());
	}

}
void Lua::Initialize()
{
	HookEnabler hook;
	ENABLER_CREATEHOOK(
		hook, (LPVOID)0x140333040, &luaB_print_f, reinterpret_cast<LPVOID*>(&origLuaBPrintFunction));
}
