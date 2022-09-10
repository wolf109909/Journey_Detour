#pragma once

#ifndef __GUI_CONSOLE_H__
#define __GUI_CONSOLE_H__
#include "stdafx.h"

//#include "custommenu.h"
namespace GUIConsole 
{
	typedef void(__cdecl* _debug_print)(const char* a1, ...);

	extern bool redirectconsoleoutput;

	void Initialize();
	void AppendLine(std::string line);
	void Toggle();
	void Draw();
};
#endif