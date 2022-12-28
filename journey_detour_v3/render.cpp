#include "framework.h"
#include <windows.h>
#include <psapi.h>
#include <set>
#include <map>
#include <filesystem>
#include <sstream>
#include "spdlog/spdlog.h"
#include "MinHook.h"
#include "hooks.h"
#include "memory.h"
#include "game.h"

namespace RenderTest
{

	unsigned int color = 0xFFFFFF00;
	float offset_x = -1.5;
	float offset_y = 0.75;
	float font_size = 0.03;
	int i = 0;
	void RenderTest()
	{
		i = 0;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->XPos).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->YPos).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->ZPos).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->XAccel).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->YAccel).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->ZAccel).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->ScarfCharge).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
		g_pGame->r_AddText(g_pGame->m_Game->Render, std::to_string(g_pGame->m_LocalDude->ScarfMax).c_str(), offset_x, offset_y - font_size * i, font_size, color); i++;
	}
	void Initialize()
	{
		g_pGame->AddRenderCallback(new RenderCallback("TEST", &RenderTest));
	}

}

