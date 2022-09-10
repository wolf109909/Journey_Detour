//
// Created by scrci on 2022/9/10.
//

#include "game.h"

typedef int GetLevelUidType(Lua::lua_State *, char *LevelName);

uintptr_t Game::BaseGame = 0;
uintptr_t Game::LuaState = 0;
uintptr_t Game::Matchmaker = 0;
GetLevelUidType *GetLevelUid = (GetLevelUidType *) Global::Bases::GetLevelUid;
uintptr_t Game::Render = 0;
Game::_tick Game::Tick;

bool Game::locateGame = false;
bool Game::autolobbybool1;
bool Game::autolobbybool2;
unsigned __int8 Game::someinteger;

void Game::UpdateValues() {
    LuaState = *(int *)(BaseGame + 32);
    spdlog::info("Game::LuaState : {}", LuaState);
    Render = *(int *) (BaseGame + 192);
    spdlog::info("Game::Render : {}", Render);
    Matchmaker = *(int *) (BaseGame + 384);
    spdlog::info("Game::Matchmaker : {}", Matchmaker);
    //someinteger = *(unsigned __int8 *) (Matchmaker + 1536);
    //autolobbybool1 = *(bool *) (someinteger + 1057);
    //autolobbybool2 = *(bool *) (someinteger + 1058);

}

char *Game::CurrentLevelName() {
    return (char *) (BaseGame + 1835760);
}

int Game::CurrentLevelUid() {
    return GetLevelUid(Lua::lua_State_ptr, CurrentLevelName());
}

void Game::SetBaseAddress(uintptr_t addr) {
    if (!locateGame) {
        BaseGame = addr;
        UpdateValues();
    }
}

void PreGameTick(__int64 mGame) {
    Lua::ExecuteBuffer();
    //std::cout << mGame << std::endl;
    //AddText(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
    //AddText((gamestructbase + 192), "I live in Pre-Game::Update", -0.2, -0.1, 0.05, 0xFF0000FF);
}


void TickHook(__int64 mGame, float a2) {

    if(Game::BaseGame == 0)
    {
        spdlog::info("Game: Obtaining Game");
        Game::BaseGame = mGame;
        spdlog::info("Game::BaseGame : {}", Game::BaseGame);
        Game::UpdateValues();
    }
    else 
    {
        PreGameTick(mGame);
        Game::Tick(mGame, a2);
    }

    //std::cout << mGame << std::endl;
    

    Game::Tick(mGame, a2);
    //AddText(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
}

void Game::Initialize(HookEnabler hook){
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Global::Bases::GameTick, &TickHook, reinterpret_cast<LPVOID *>(&Game::Tick));
}
