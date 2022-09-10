//
// Created by scrci on 2022/9/10.
//
#pragma once
#ifndef CL1_MENU_H
#define CL1_MENU_H
#include <map>
#include "sharedlibrary.h"

namespace MenuSystem{
    typedef void (*Action)(void); // function pointer type
    class Item{
    private:
        std::string ItemName;
        Action function;
    public:
        Item(std::string name, Action function);
        void Execute();
        std::string GetName();
    };
    extern int selectedItem;
    extern std::map<int, Item *> MenuItems;
    void MenuSelectionChanged();
    void SelectPreviousItem();
    void SelectNextItem();
    void ExecuteItem();
    void AddItem(std::string itemname, Action function);
    namespace Actions{
        void Test();
        void ToggleNetGui();
        void ToggleConsole();
        void CycleDebugHud();
        void SetMaxOutfit();
        void SetMaxCloth();
        void FillScarf();
        void GotoNick();
        void ToggleMusic();
    }
    void InitializeMenu();
    namespace GUI{
        void Draw();
    }
}
#endif //CL1_MENU_H
