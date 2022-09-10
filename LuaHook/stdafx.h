// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef __PRECOMPILE_HEADER_H__
#define __PRECOMPILE_HEADER_H__
#include "targetver.h"
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>



// reference additional headers your program requires here
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <functional>
#include <map>
#include <set>
#include <vector>
#include <filesystem>


#include <stdio.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <Psapi.h>


#include "spdlog/spdlog.h"
#include "MinHook.h"

#endif