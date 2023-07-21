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

OFFSET_STRUCT(Game)
{
	STRUCT_SIZE(0x1C0381)
		FIELD(0x28,uintptr_t ResourceManager)
		FIELD(0xC0, uintptr_t Render)
		FIELD(0x138,uintptr_t DecorationBarn)
		FIELD(0x180, uintptr_t Network)
		FIELD(0x1E0, uintptr_t PlayerBarn)
};

OFFSET_STRUCT(LocalDude) 
{
	STRUCT_SIZE(0x1C01A1)
		FIELD(0xC0, float XPos)
		FIELD(0xC4, float YPos)
		FIELD(0xC8, float ZPos)
		FIELD(0xD4, float XAccel)
		FIELD(0xD8, float YAccel)
		FIELD(0xDC, float ZAccel)
		FIELD(0x118, int ScarfCharge)
		FIELD(0x11C, int ScarfMax)
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