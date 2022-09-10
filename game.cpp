//
// Created by scrci on 2022/9/10.
//

#include "game.h"

typedef int GetLevelUidType(Lua::lua_State *, char *LevelName);

uintptr_t game;
uintptr_t Game::render;
uintptr_t Game::matchmaker;
GetLevelUidType *GetLevelUid = (GetLevelUidType *) Global::Bases::GetLevelUid;
uintptr_t Game::GameRender = 0;
Game::_tick Game::Tick;

bool Game::locateGame = false;
bool Game::autolobbybool1;
bool Game::autolobbybool2;
unsigned __int8 Game::someinteger;

void Game::UpdateValues() {
    render = *(int *) (game + 192);
    matchmaker = *(int *) (game + 384);
    someinteger = *(unsigned __int8 *) (matchmaker + 1536);
    autolobbybool1 = *(bool *) (someinteger + 1057);
    autolobbybool2 = *(bool *) (someinteger + 1058);

}

char *Game::CurrentLevelName() {
    return (char *) (game + 1835760);
}

int Game::CurrentLevelUid() {
    return GetLevelUid(Lua::lua_State_ptr, CurrentLevelName());
}

void Game::SetBaseAddress(uintptr_t addr) {
    if (!locateGame) {
        game = addr;
        UpdateValues();
    }
}

void PreGameTick(__int64 mGame) {
    Lua::ExecuteBuffer();
    //std::cout << mGame << std::endl;
    //AddText(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
    //AddText((gamestructbase + 192), "I live in Pre-Game::Update", -0.2, -0.1, 0.05, 0xFF0000FF);
}
uintptr_t gamestructbase = 0;

void TickHook(__int64 mGame, float a2) {
    gamestructbase = mGame;
    Game::GameRender = *(int *) (mGame + 192);
    //std::cout << mGame << std::endl;
    PreGameTick(mGame);

    Game::Tick(mGame, a2);
    //AddText(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
}

void Game::Initialize(HookEnabler hook){
    ENABLER_CREATEHOOK(
            hook, (LPVOID) Global::Bases::GameTick, &TickHook, reinterpret_cast<LPVOID *>(&Game::Tick));
}
