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
//const char* IMAGE_NAME = "journey.exe.unpacked.exe";


void __fileAutohook::Dispatch()
{
	for (__autohook* hook : hooks)
		hook->Dispatch();
}

void __fileAutohook::DispatchForModule(const char* pModuleName)
{
	const int iModuleNameLen = strlen(pModuleName);

	for (__autohook* hook : hooks)
		if ((hook->iAddressResolutionMode == __autohook::OFFSET_STRING && !strncmp(pModuleName, hook->pAddrString, iModuleNameLen)) ||
			(hook->iAddressResolutionMode == __autohook::PROCADDRESS && !strcmp(pModuleName, hook->pModuleName)))
			hook->Dispatch();
}

ManualHook::ManualHook(const char* funcName, LPVOID func) : pHookFunc(func), ppOrigFunc(nullptr)
{
	const int iFuncNameStrlen = strlen(funcName);
	pFuncName = new char[iFuncNameStrlen];
	memcpy(pFuncName, funcName, iFuncNameStrlen);
}

ManualHook::ManualHook(const char* funcName, LPVOID* orig, LPVOID func) : pHookFunc(func), ppOrigFunc(orig)
{
	const int iFuncNameStrlen = strlen(funcName);
	pFuncName = new char[iFuncNameStrlen];
	memcpy(pFuncName, funcName, iFuncNameStrlen);
}

bool ManualHook::Dispatch(LPVOID addr, LPVOID* orig)
{
	if (orig)
		ppOrigFunc = orig;

	if (MH_CreateHook(addr, pHookFunc, ppOrigFunc) == MH_OK)
	{
		if (MH_EnableHook(addr) == MH_OK)
		{
			spdlog::info("Enabling hook {}", pFuncName);
			return true;
		}
		else
			spdlog::error("MH_EnableHook failed for function {}", pFuncName);
	}
	else
		spdlog::error("MH_CreateHook failed for function {}", pFuncName);

	return false;
}

void MakeHook(LPVOID pTarget, LPVOID pDetour, void* ppOriginal, const char* pFuncName)
{
	char* pStrippedFuncName = (char*)pFuncName;
	// strip & char from funcname
	if (*pStrippedFuncName == '&')
		pStrippedFuncName++;

	if (MH_CreateHook(pTarget, pDetour, (LPVOID*)ppOriginal) == MH_OK)
	{
		if (MH_EnableHook(pTarget) == MH_OK)
			spdlog::info("Enabling hook {}", pStrippedFuncName);
		else
			spdlog::error("MH_EnableHook failed for function {}", pStrippedFuncName);
	}
	else
		spdlog::error("MH_CreateHook failed for function {}", pStrippedFuncName);
}







void InstallInitialHooks()
{
	if (MH_Initialize() != MH_OK)
		spdlog::error("MH_Initialize (minhook initialization) failed");
}
