#pragma once
#ifndef __RENDER_H__
#define __RENDER_H__
namespace Render 
{
	typedef int(__cdecl* _addtext)(__int64 a1, const char* text, float x, float y, float size, int color);
	typedef void(__cdecl* _netgui)(__int64 a1, __int64 a2, float a3);
	extern _netgui origNetGuiFunction;
	extern _addtext Addtext;
	void Initialize();
};
#endif