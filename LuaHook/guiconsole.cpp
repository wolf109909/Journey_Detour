#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "guiconsole.h"
#include "game.h"
#include "render.h"
#include "hookutils.h"
//#include "guiconsole.h"
//#include "hookutils.h"
//#include "render.h"
//#include "game.h"
namespace GUIConsole
{
	_debug_print origTargetdebugprintFunction = NULL;

	float GUIConsole_X = -1.75;
	float GUIConsole_y = 0;
	float GUIConsole_charsize = 0.04;
	const int GUIConsole_Width = 120;
	const int GUIConsole_Height = 30;
	int CountDown = 500;
	int CountDownLimit = 100;
	int GUIConsole_Cursor = 0;
	bool redirectconsoleoutput = true;

	//char GUIConsole_Buffer[GUIConsole_Width * GUIConsole_Height];
	std::vector<std::string> console_lines;


	void Toggle()
	{
		if (redirectconsoleoutput)
		{
			std::cout << "[*] Disabled game print redirection" << std::endl;
		}
		else
		{
			std::cout << "[*] Enabled game print redirection" << std::endl;
		}
		redirectconsoleoutput = !redirectconsoleoutput;
	}
	void AppendLine(std::string line)
	{
		if (console_lines.size() > GUIConsole_Height)
		{
			console_lines.erase(console_lines.begin());
		}
		CountDown = 500;
		console_lines.push_back(line);
	}
	void Draw()
	{
		if (!redirectconsoleoutput)
			return;

		CountDown--;
		int linenum = 0;
		for (auto line : console_lines)
		{
			float x = GUIConsole_X;
			float y = -(linenum * GUIConsole_charsize * 0.75);
			if (CountDown < CountDownLimit)
			{
				CountDown = 0;
				Render::Addtext(Game::BaseRender, line.c_str(), x, y, GUIConsole_charsize, 0x30FFFFFF);
			}
			else
			{
				Render::Addtext(Game::BaseRender, line.c_str(), x, y, GUIConsole_charsize, 0xA0FFFFFF);
			}

			//Addtext(gamerender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
			linenum++;
		}

	}
	const char* __cdecl hooked_debug_print(const char* fmt, ...)
	{
		if (!redirectconsoleoutput)
		{
			return fmt;
		}
		va_list args;
		char buffer[16384];
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		va_end(args);
		spdlog::info(buffer);
		AppendLine(buffer);
		return fmt;
	}

	void Initialize()
	{
		HookEnabler hook;
		ENABLER_CREATEHOOK(
			hook, (LPVOID)0x1402E3090, &hooked_debug_print, reinterpret_cast<LPVOID*>(&origTargetdebugprintFunction));
	}
};