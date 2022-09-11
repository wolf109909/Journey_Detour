//
// Created by scrci on 2022/9/10.
//

#include "menu.h"
#include <utility>
#include "lua.h"
#include "utils.h"
#include "game.h"


bool MenuParsed = false;
int selectedItem = 0;
//std::map<int, Menu::Page *> MenuPages;


std::string Menu::Page::GetName() 
{
    return this->PageName;
}

Menu::Page* GetPageByName(std::string name) 
{
    for (Menu::Page* pageptr : Menu::Pages) 
    {
        Menu::Page page = *pageptr;
        spdlog::info("search:{},target:{}", page.GetName(), name);
        if (page.GetName() == name)
            return pageptr;
    }
    spdlog::warn("GetPageByName(): Can't find page with name {}",name);
}

Menu::Page* Menu::GetActivePage()
{
    return Menu::ActivePage;
}

void Menu::ChangeActivePage(Page* page)
{
    Menu::ActivePage = page;
    selectedItem = 0;
}

void onSelectionChanged() {
    if (selectedItem < 0)
        selectedItem = Menu::GetActivePage()->GetItems().size() - 1;
    if (selectedItem > Menu::GetActivePage()->GetItems().size() - 1)
        selectedItem = 0;
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
    Menu::GetActivePage()->GetItems()[selectedItem]->Execute();
}



void Menu::Initialize() {
    Menu::Page* mainmenu = new Menu::Page("mainmenu");
        mainmenu->InsertItem(new Menu::Item("ToggleNetGui", &Actions::ToggleNetGui));
        mainmenu->InsertItem(new Menu::Item("ToggleConsole", &Actions::ToggleConsole));
        mainmenu->InsertItem(new Menu::Item("CycleDebugHud", &Actions::CycleDebugHud));
        mainmenu->InsertItem(new Menu::Item("SetMaxOutfit", &Actions::SetMaxOutfit));
        mainmenu->InsertItem(new Menu::Item("SetMaxCloth", &Actions::SetMaxCloth));
        mainmenu->InsertItem(new Menu::Item("FillScarf", &Actions::FillScarf));
        mainmenu->InsertItem(new Menu::Item("GotoNick", &Actions::GotoNick));
        mainmenu->InsertItem(new Menu::Item("ToggleMusic", &Actions::ToggleMusic));
        
        Menu::ChangeActivePage(GetPageByName("mainmenu"));

        MenuParsed = true;
}

void Menu::Actions::GotoMainMenu() 
{
    Menu::ChangeActivePage(GetPageByName("mainmenu"));
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


Menu::Page::Page(std::string name)
{
    this->Items;
    this->PageName = name;
    Pages.push_back(this);
}

void Menu::Page::InsertItem(Item* item) 
{
    this->Items.push_back(item);
}

std::vector<Menu::Item*> Menu::Page::GetItems()
{
    return this->Items;
}

namespace Menu::GUI {
    float offset_x = 1.2;
    float offset_y = 0.75;
    float font_size = 0.05;
    unsigned int color = 0xFFFFFF00;
    unsigned int color_picked = 0xFF0000FF;

    void Draw() {
        if (!MenuParsed) 
        {
            return;
        }

        int id = 0;
        for (auto item : Menu::GetActivePage()->GetItems()) {
            //Item item = *item;
            float x = offset_x;
            float y = offset_y - font_size * id;
            //std::string item_name = item->GetName();
            //spdlog::info(item.GetName().c_str());
            if (id == selectedItem) {
                Render::AddText(Game::Render, item->GetName().c_str(), x, y, font_size, color_picked);
            } else {
                Render::AddText(Game::Render, item->GetName().c_str(), x, y, font_size, color);
            }

            id++;
            //spd
            // log::info("X: {} Y:{} NAME: {} SIZE: {} COLOR: {}", x, y, item_name, font_size, color);
            //Sleep(1000);
            //AddText(GameRender, std::to_string(CountDown).c_str() , 0, 0, GUIConsole_charsize, 0xFF000000);
        }

    }
}