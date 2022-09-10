//
// Created by scrci on 2022/9/10.
//

#include "game.h"
uintptr_t Game::game;
uintptr_t Game::render;
uintptr_t Game::matchmaker;
Game::GetLevelUidType* Game::GetLevelUid = (Game::GetLevelUidType*)0x1400DA830;
bool Game::locateGame = false;
bool Game::autolobbybool1;
bool Game::autolobbybool2;
unsigned __int8 Game::someinteger;
    void Game::UpdateValues()
    {
        render = *(int*)(game + 192);
        matchmaker = *(int*)(game + 384);
        someinteger = *(unsigned __int8*)(matchmaker + 1536);
        autolobbybool1 = *(bool*)(someinteger + 1057);
        autolobbybool2 = *(bool*)(someinteger + 1058);

    }

    char* Game::CurrentLevelName()
    {
        return (char*)(game + 1835760);
    }
    int Game::CurrentLevelUid()
    {
        return GetLevelUid(Lua::lua_State_ptr, CurrentLevelName());
    }
    void Game::SetBaseAddress(uintptr_t addr)
    {
        if (!locateGame)
        {
            game = addr;
            UpdateValues();
        }

    }
