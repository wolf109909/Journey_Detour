//
// Created by scrci on 2022/9/10.
//
#include "hookutils.h"
#include "utils.h"
#include "game.h"
Game::GetLevelUidType* Game::GetLevelUid = (Game::GetLevelUidType*)0x1400DA830;
bool Game::locateGame = false;
bool Game::autolobbybool1;
bool Game::autolobbybool2;
unsigned __int8 Game::someinteger;
    void Game::UpdateValues()
    {
        Global::BaseRender = *(int*)(Global::BaseGame + 192);
        Global::BaseMatchmaker = *(int*)(Global::BaseGame + 384);
        
       // someinteger = *(unsigned __int8*)(BaseMatchmaker + 1536);
        //autolobbybool1 = *(bool*)(someinteger + 1057);
       //autolobbybool2 = *(bool*)(someinteger + 1058);

    }

    void Game::PreGameTick(__int64 game)
    {
        Lua::ExecuteBuffer();
        //std::cout << game << std::endl;
        //Addtext(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
        //Addtext((gamestructbase + 192), "I live in Pre-Game::Update", -0.2, -0.1, 0.05, 0xFF0000FF);
    }
    void Game::GameTick(__int64 game, float a2)
    {
        if(Global::BaseGame == 0)
        { 
            Global::BaseGame = game; 
        }
        else 
        { 
            PreGameTick(game); 
        }

        origGameTickFunction(game, a2);
        //Addtext(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
    }

    char* Game::CurrentLevelName()
    {
        return (char*)(Global::BaseGame + 1835760);
    }
    int Game::CurrentLevelUid()
    {
        return GetLevelUid(Lua::lua_State_ptr, CurrentLevelName());
    }
    void Game::Initialize()
    {

        HookEnabler hook;


    }
