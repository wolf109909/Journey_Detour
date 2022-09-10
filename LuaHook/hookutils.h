#pragma once
#ifndef __HOOK_UTILS_H__
#define __HOOK_UTILS_H__
//#include <vector>
#include "stdafx.h"
//#include <windows.h>
//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <stdio.h>
//#include <stdlib.h>
//#include "MinHook.h"
//#include <tlhelp32.h>
//#include <Psapi.h>//include in stdafx
// Enables all hooks created with the HookEnabler object when it goes out of scope and handles hook errors
class HookEnabler
{
private:
	struct HookTarget
	{
		char* targetName;
		LPVOID targetAddress;
	};

	std::vector<HookTarget*> m_hookTargets;

public:
	void CreateHook(LPVOID ppTarget, LPVOID ppDetour, LPVOID* ppOriginal, const char* targetName = nullptr);
	~HookEnabler();
};

// macro to call HookEnabler::CreateHook with the hook's name
#define ENABLER_CREATEHOOK(enabler, ppTarget, ppDetour, ppOriginal)                                                                        \
	enabler.CreateHook(ppTarget, reinterpret_cast<void*>(ppDetour), ppOriginal, #ppDetour)

#endif