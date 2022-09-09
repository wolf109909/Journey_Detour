#pragma once
#include <vector>
#include <map>
#include <string>
#include "lua.h"
//#include "guiconsole.h"
#include "game.h"
#include "render.h"
namespace MenuSystem
{
	typedef void (*Action)(void); // function pointer type

	class Item
	{
	private:
		std::string ItemName;
		Action function;
	public:
		Item(std::string name, Action function)
		{
			this->ItemName = name;
			this->function = function;
		}
		void Execute()
		{
			this->function();
		}
		std::string GetName()
		{
			return this->ItemName;
		}
	};

	int selectedItem = 0;
	std::map<int, Item*> MenuItems;



	void MenuSelectionChanged()
	{
		if (selectedItem < 0)
			selectedItem = 0;
		if (selectedItem > MenuItems.size() - 1)
			selectedItem = MenuItems.size() - 1;
	}
	void SelectPreviousItem()
	{
		selectedItem--;
		MenuSelectionChanged();
	}

	void SelectNextItem()
	{
		selectedItem++;
		MenuSelectionChanged();
	}

	void ExecuteItem()
	{
		Item item = *MenuItems[selectedItem];
		item.Execute();
	}

	void AddItem(std::string itemname, Action function)
	{
		Item* item = new Item(itemname, function);
		int id = MenuItems.size();
		spdlog::info("Adding menu item {},id {}", itemname, id);
		MenuItems.insert_or_assign(id, item);
	}

	void Action_Test()
	{
		Lua::AppendBuffer("game:QueueLevel(\"Level_Desert\")");
	}

	void Action_ToggleNetGui()
	{
		Lua::AppendBuffer("game:netGui():ToggleEnabled()");
		Game::isNetGUIEnabled = !Game::isNetGUIEnabled;
	}
	void Action_ToggleConsole()
	{
		GUIConsole::redirectconsoleoutput = !GUIConsole::redirectconsoleoutput;
	}
	void Action_CycleDebugHud()
	{
		Lua::AppendBuffer("game:debugHud():CycleMode()");
	}
	void Action_SetMaxOutfit()
	{
		Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetOutfit(8)");
	}
	void Action_SetMaxCloth()
	{
		Lua::AppendBuffer("SpawnEvent{ SetMaxCloth = { changeAmount = 32 } }");
	}
	void Action_FillScarf()
	{
		Lua::AppendBuffer("game:playerBarn():GetLocalDude():FillScarf(32, game:soundBarn())");
	}
	void Action_GotoNick()
	{
		Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetPos(game:playerBarn():GetRemoteDude():GetPos())");
	}
	void Action_ToggleMusic()
	{
		Lua::AppendBuffer("game:soundBarn():ToggleMuteMusic()");
	}



	void InitializeMenu()
	{
		AddItem("ToggleNetGui", &Action_ToggleNetGui);
		AddItem("ToggleConsole", &Action_ToggleConsole);
		AddItem("CycleDebugHud", &Action_CycleDebugHud);
		AddItem("SetMaxOutfit", &Action_SetMaxOutfit);
		AddItem("SetMaxCloth", &Action_SetMaxCloth);
		AddItem("FillScarf", &Action_FillScarf);
		AddItem("GotoNick", &Action_GotoNick);
		AddItem("ToggleMusic", &Action_ToggleMusic);

	}

	namespace GUI
	{
		float xoffset = 1.2;
		float yoffset = 0.75;
		float fontsize = 0.05;
		unsigned int color = 0xFFFFFF00;
		unsigned int color_picked = 0xFF0000FF;

		void Draw()
		{
			for (auto mapitem : MenuItems)
			{
				//Item item = *item;
				int itemid = mapitem.first;
				Item item = *mapitem.second;
				float x = xoffset;
				float y = yoffset - fontsize * itemid;
				std::string itemname = item.GetName();
				//spdlog::info(item.GetName().c_str());
				if (itemid == selectedItem)
				{
					Render::Addtext(Game::BaseRender, itemname.c_str(), x, y, fontsize, color_picked);
				}
				else
				{
					Render::Addtext(Game::BaseRender, itemname.c_str(), x, y, fontsize, color);
				}
				//spdlog::info("X: {} Y:{} NAME: {} SIZE: {} COLOR: {}", x, y, itemname, fontsize, color);
				//Sleep(1000);
				//Addtext(gamerender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
			}

		}
	}
};