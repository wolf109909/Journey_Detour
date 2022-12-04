
#include"offsets.h"
#include "game.h"

typedef int GetLevelUidType(Lua::lua_State *, char *LevelName);

uintptr_t Game::BaseGame = 0;
uintptr_t Game::LuaState = 0;
uintptr_t Game::Matchmaker = 0;
uintptr_t Game::PlayerBarn = 0;
float* Game::LocalDude::XPos = 0;
float* Game::LocalDude::YPos = 0;
float* Game::LocalDude::ZPos = 0;

float* Game::LocalDude::XAccel = 0;
float* Game::LocalDude::YAccel = 0;
float* Game::LocalDude::ZAccel = 0;

int* Game::LocalDude::ScarfCharge = 0;
int* Game::LocalDude::ScarfMax = 0;

//uintptr_t Game::LocalDude = 0;
GetLevelUidType *GetLevelUid = (GetLevelUidType *) g_Offsets->GetLevelUid;
uintptr_t Game::Render = 0;
Game::_tick Game::Tick;


bool* Game::autolobbybool1;
bool* Game::autolobbybool2;
unsigned __int8 Game::someinteger;

void Game::UpdateValues() {
    LuaState = *(int *)(BaseGame + 32);
    spdlog::info("Game::LuaState : {}", LuaState);
    Render = *(int *) (BaseGame + 192);
    spdlog::info("Game::Render : {}", Render);
    Matchmaker = *(int *) (BaseGame + 384);
    spdlog::info("Game::Matchmaker : {}", Matchmaker);

    PlayerBarn = *(int*)(BaseGame + 480);
    spdlog::info("Game::PlayerBarn : {}", PlayerBarn);


    Game::LocalDude::XPos = (float*)(Game::PlayerBarn + 0xC0);
	Game::LocalDude::YPos = (float*)(Game::PlayerBarn + 0xC4);
	Game::LocalDude::ZPos = (float*)(Game::PlayerBarn + 0xC8);

	Game::LocalDude::XAccel = (float*)(Game::PlayerBarn + 0xD4);
	Game::LocalDude::YAccel = (float*)(Game::PlayerBarn + 0xD8);
	Game::LocalDude::ZAccel = (float*)(Game::PlayerBarn + 0xDC);

	Game::LocalDude::ScarfCharge = (int*)(Game::PlayerBarn + 0x118);
	Game::LocalDude::ScarfMax = (int*)(Game::PlayerBarn + 0x11C);
    //LocalDude = *(int*)(BaseGame + 480 + 176);
    //spdlog::info("Game::LocalDude : {}", LocalDude);
    //RemoteDude = *(int*)(BaseGame + 480);
    //spdlog::info("Game::RemoteDude : {}", RemoteDude);
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

    BaseGame = addr;
    UpdateValues();
    
}

void PreGameTick(__int64 mGame) {
    Lua::ExecuteBuffer();
}


void TickHook(__int64 mGame, float a2) {
    //spdlog::info(a2);
    if(Game::BaseGame == 0)
    {
        spdlog::info("Game: Obtaining Game");
        Game::BaseGame = mGame;
        spdlog::info("Game::BaseGame : {}", Game::BaseGame);
        Game::UpdateValues();
        Game::Tick(mGame, a2);
    }
    else 
    {
        PreGameTick(mGame);
        Game::Tick(mGame, a2);
        
    }

    //std::cout << mGame << std::endl;
    

    //Game::Tick(mGame, a2);
    //AddText(gamestructbase + 192, "TESTTEST", 0, 0, (unsigned int)0x14068AFF4, (float)0xFFFF);
}

void Game::Initialize(HookEnabler hook){
    ENABLER_CREATEHOOK(
            hook, (LPVOID) g_Offsets->GameTick, &TickHook, reinterpret_cast<LPVOID *>(&Game::Tick));
}
