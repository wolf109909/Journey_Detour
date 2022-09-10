//
// Created by scrci on 2022/9/10.
//

#include "menu.h"

#include <utility>
#include "lua.h"
#include "utils.h"
#include "game.h"

int selectedItem = 0;
std::map<int, Menu::Item *> MenuItems;

void onSelectionChanged() {
    if (selectedItem < 0)
        selectedItem = 0;
    if (selectedItem > MenuItems.size() - 1)
        selectedItem = MenuItems.size() - 1;
}

void Menu::SelectPreviousItem() {
    selectedItem--;
    onSelectionChanged();
}

void Menu::SelectNextItem() {
    selectedItem++;
    onSelectionChanged();
}

void Menu::ExecuteItem() {
    Item item = *MenuItems[selectedItem];
    item.Execute();
}

void Menu::AddItem(const std::string& itemName, Action function) {
    Item *item = new Item(itemName, function);
    int id = MenuItems.size();
    spdlog::info("Adding menu item {},id {}", itemName, id);
    MenuItems.insert_or_assign(id, item);
}

void Menu::Initialize() {
    AddItem("ToggleNetGui",     &Actions::Test);
    AddItem("ToggleConsole",    &Actions::ToggleConsole);
    AddItem("CycleDebugHud",    &Actions::CycleDebugHud);
    AddItem("SetMaxOutfit",     &Actions::SetMaxOutfit);
    AddItem("SetMaxCloth",      &Actions::SetMaxCloth);
    AddItem("FillScarf",        &Actions::FillScarf);
    AddItem("GotoNick",         &Actions::GotoNick);
    AddItem("ToggleMusic",      &Actions::ToggleMusic);

}

void Menu::Actions::Test() {
    Lua::AppendBuffer("game:QueueLevel(\"Level_Desert\")");
}

void Menu::Actions::ToggleNetGui() {
    Lua::AppendBuffer("game:netGui():ToggleEnabled()");
    Global::isNetGUIEnabled = !Global::isNetGUIEnabled;
}

void Menu::Actions::ToggleConsole() {
    Global::console_OutputRedirect = !Global::console_OutputRedirect;
}

void Menu::Actions::CycleDebugHud() {
    Lua::AppendBuffer("game:debugHud():CycleMode()");
}

void Menu::Actions::SetMaxOutfit() {
    Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetOutfit(8)");
}

void Menu::Actions::SetMaxCloth() {
    Lua::AppendBuffer("SpawnEvent{ SetMaxCloth = { changeAmount = 32 } }");
}

void Menu::Actions::FillScarf() {
    Lua::AppendBuffer("game:playerBarn():GetLocalDude():FillScarf(32, game:soundBarn())");
}

void Menu::Actions::GotoNick() {
    Lua::AppendBuffer("game:playerBarn():GetLocalDude():SetPos(game:playerBarn():GetRemoteDude():GetPos())");
}

void Menu::Actions::ToggleMusic() {
    Lua::AppendBuffer("game:soundBarn():ToggleMuteMusic()");
}

Menu::Item::Item(std::string name, Menu::Action function) {
    this->ItemName = std::move(name);
    this->function = function;
}

void Menu::Item::Execute() {
    this->function();
}

std::string Menu::Item::GetName() {
    return this->ItemName;
}

namespace Menu::GUI {
    float offset_x = 1.2;
    float offset_y = 0.75;
    float font_size = 0.05;
    unsigned int color = 0xFFFFFF00;
    unsigned int color_picked = 0xFF0000FF;

    void Draw() {
        for (auto item_map: MenuItems) {
            //Item item = *item;
            int item_id = item_map.first;
            Item item = *item_map.second;
            float x = offset_x;
            float y = offset_y - font_size * item_id;
            std::string item_name = item.GetName();
            //spdlog::info(item.GetName().c_str());
            if (item_id == selectedItem) {
                Render::AddText(Game::Render, item_name.c_str(), x, y, font_size, color_picked);
            } else {
                Render::AddText(Game::Render, item_name.c_str(), x, y, font_size, color);
            }
            //spdlog::info("X: {} Y:{} NAME: {} SIZE: {} COLOR: {}", x, y, item_name, font_size, color);
            //Sleep(1000);
            //AddText(GameRender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
        }

    }
}