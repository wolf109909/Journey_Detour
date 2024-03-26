#pragma once
#include "memory.h"
#include "structs.h"

struct vec_mat {
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
};

typedef int (*_addtext)(__int64 a1, const char* text, float x, float y, float size, int color);
typedef __int64(__cdecl* _getconsole)(char*);
typedef __int64(__cdecl* _printluamem)(__int64);
typedef void(__cdecl* _adddeco)(int a1, int a2, char* a3, int a4, vec_mat* a5);




union Game
{
	struct {
		char pad31[0x28]; uintptr_t ResourceManager;
	};
	struct {
		char pad34[0xC0]; uintptr_t Render;
	};
	struct {
		char pad37[0x138]; uintptr_t DecorationBarn;
	};
	struct {
		char pad40[0x180]; uintptr_t Network;
	};
	struct {
		char pad43[0x1E0]; uintptr_t PlayerBarn;
	};
};

union LocalDude
{
	struct {
		char pad41[0xC0]; float XPos;
	};
	struct {
		char pad44[0xC4]; float YPos;
	};
	struct {
		char pad47[0xC8]; float ZPos;
	};
	struct {
		char pad50[0xD4]; float XAccel;
	};
	struct {
		char pad53[0xD8]; float YAccel;
	};
	struct {
		char pad56[0xDC]; float ZAccel;
	};
	struct {
		char pad59[0x118]; int ScarfCharge;
	};
	struct {
		char pad62[0x11C]; int ScarfMax;
	};
};


typedef void (*RenderContext)();
class RenderCallback
{
private:
	std::string m_Name;
	RenderContext m_Context;
public:
	RenderCallback(std::string name, RenderContext context);
	void Remove(std::string name);
	void Execute();
};

class GameManager
{
public:
	std::vector<RenderCallback*> m_vRenderCallbacks;

	_addtext r_AddText;
	_getconsole g_GetConsole;
	_printluamem g_PrintLuaMem;
	_adddeco g_AddDecoration;


	uintptr_t p_lastDecoration;

	bool b_Initialized = false;
	bool b_IsNetGUIEnabled = false;
	bool b_IsConsoleRedirectEnabled = false;
	
	// cheats
	bool b_InfiniteScarf = false;


	Game* m_Game;
	LocalDude* m_LocalDude;
	// Initialize hooks
	void Initialize();
	void AddRenderCallback(RenderCallback* callback);
	void ToggleInfiniteScarf();
	char* GetPartnerName();
};
extern GameManager* g_pGame;