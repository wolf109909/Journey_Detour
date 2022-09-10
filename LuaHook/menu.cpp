//
// Created by scrci on 2022/9/10.
//

#include "menu.h"
#include "lua.h"
#include "utils.h"

int MenuSystem::selectedItem = 0;
std::map<int, MenuSystem::Item*> MenuSystem::MenuItems;

void MenuSystem::MenuSelectionChanged() {
    if (selectedItem < 0)
        selectedItem = 0;
    if (selectedItem > MenuItems.size() - 1)
        selectedItem = MenuItems.size() - 1;
}

void MenuSystem::SelectPreviousItem() {
    selectedItem--;
    MenuSelectionChanged();
}

void MenuSystem::SelectNextItem() {
    selectedItem++;
    MenuSelectionChanged();
}

void MenuSystem::ExecuteItem() {
    Item item = *MenuItems[selectedItem];
    item.Execute();
}

void MenuSystem::AddItem(std::string itemname, Action function) {
    Item *item = new Item(itemname, function);
    int id = MenuItems.size();
    spdlog::info("Adding menu item {},id {}", itemname, id);
    MenuItems.insert_or_assign(id, item);
}

void MenuSystem::Actions::Test() {
    Lua::AppendBuffer("game:QueueLevel(\"Level_Desert\")");
}

void MenuSystem::Actions::ToggleNetGui() {
    Lua::AppendBuffer("game:netGui():ToggleEnabled()");
    Global::isNetGUIEnabled = !Global::isNetGUIEnabled;
}

void MenuSystem::Actions::ToggleConsole() {
    Global::redirectconsoleoutput = !Global::redirectconsoleoutput;
}

void MenuSystem::Actions::CycleDebugHud() {
    Lua::AppendBuffer("game:debugHud():CycleMode()");
}

void MenuSystem::Actions::SetMaxOutfit() {
    Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetOutfit(8)");
}

void MenuSystem::Actions::SetMaxCloth() {
    Lua::AppendBuffer("SpawnEvent{ SetMaxCloth = { changeAmount = 32 } }");
}

void MenuSystem::Actions::FillScarf() {
    Lua::AppendBuffer("game:playerBarn():GetLocalDude():FillScarf(32, game:soundBarn())");
}

void MenuSystem::Actions::GotoNick() {
    Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetPos(game:playerBarn():GetRemoteDude():GetPos())");
}

void MenuSystem::Actions::ToggleMusic() {
    Lua::AppendBuffer("game:soundBarn():ToggleMuteMusic()");
}


void MenuSystem::InitializeMenu() {
    AddItem("ToggleNetGui", &Actions::Test);
    AddItem("ToggleConsole", &Actions::ToggleConsole);
    AddItem("CycleDebugHud", &Actions::CycleDebugHud);
    AddItem("SetMaxOutfit", &Actions::SetMaxOutfit);
    AddItem("SetMaxCloth", &Actions::SetMaxCloth);
    AddItem("FillScarf", &Actions::FillScarf);
    AddItem("GotoNick", &Actions::GotoNick);
    AddItem("ToggleMusic", &Actions::ToggleMusic);

}

MenuSystem::Item::Item(std::string name, MenuSystem::Action function) {
    this->ItemName = name;
    this->function = function;
}

void MenuSystem::Item::Execute() {
    this->function();
}

std::string MenuSystem::Item::GetName() {
    return this->ItemName;
}

namespace MenuSystem::GUI {
    float xoffset = 1.2;
    float yoffset = 0.75;
    float fontsize = 0.05;
    unsigned int color = 0xFFFFFF00;
    unsigned int color_picked = 0xFF0000FF;

    void Draw() {
        for (auto mapitem: MenuItems) {
            //Item item = *item;
            int itemid = mapitem.first;
            Item item = *mapitem.second;
            float x = xoffset;
            float y = yoffset - fontsize * itemid;
            std::string itemname = item.GetName();
            //spdlog::info(item.GetName().c_str());
            if (itemid == selectedItem) {
                Global::Addtext(Global::gamerender, itemname.c_str(), x, y, fontsize, color_picked);
            } else {
                Global::Addtext(Global::gamerender, itemname.c_str(), x, y, fontsize, color);
            }
            //spdlog::info("X: {} Y:{} NAME: {} SIZE: {} COLOR: {}", x, y, itemname, fontsize, color);
            //Sleep(1000);
            //Addtext(gamerender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
        }

    }
}