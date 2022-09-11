//
// Created by scrci on 2022/9/10.
//
#pragma once
#ifndef CL1_MENU_H
#define CL1_MENU_H

#include <map>
#include "sharedlibrary.h"
#include "render.h"


namespace Menu {

    typedef void (*Action)(); // function pointer type
    class Item 
    {
    private:
        std::string ItemName;
        Action function;
    public:
        Item(std::string name, Action function);

        void Execute();

        std::string GetName();
    };

  
    class Page 
    {
    private:
        std::string PageName;
        std::vector<Item*> Items;
        //Action OnExit;
    public:
        Page(std::string name);
        void InsertItem(Item*);
        std::string GetName();
        std::vector<Item*> GetItems();
    };


    Page* ActivePage;
    std::vector<Page*> Pages;
 
   
//    extern int selectedItem;
//    extern std::map<int, Item *> MenuItems;

//    void onSelectionChanged();// no reference
    Page* GetActivePage();

    void ChangeActivePage(Page* page);

    void SelectPreviousItem();

    void SelectNextItem();

    void ExecuteItem();

    void Initialize();

    namespace Actions {

        void GotoMainMenu();
        
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

    namespace GUI {
        void Draw();
    }
}
#endif //CL1_MENU_H
