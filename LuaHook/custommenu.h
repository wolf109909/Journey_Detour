#pragma once

#ifndef __CUSTOM_MENU_H__
#define __CUSTOM_MENU_H__
namespace MenuSystem 
{
	void InitializeMenu();
	void SelectNextItem();
	void SelectPreviousItem();
	void ExecuteItem();
	namespace GUI 
	{
		void Draw();
	}
};
#endif