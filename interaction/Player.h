#ifndef PLAYER_H_
#define PLAYER_H_
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <array>
#include "item\Item.h"
#include "item\Weapon.h"
#include "item\Wearable.h"
#include "..\GameWorld\Tools.h"

#define STARTING_ATRRIBUTE_POINTS 4

using namespace std;

class Player {
    private:
        friend class GameInstance;
        vector<Item> inventory;
        int inventorySpace;
        Weapon *activeWeapon;
        Wearable *activeWearable1;
        Wearable *activeWearable2;
        int luck, strength, charisma;
        void changeInventorySpace(int);
        void changeParams(int params[], bool equip);
    public:
        Player();
        Player(string playerToken);
        int getStrength();
        int getLuck();
        int getCharisma();
        bool equip(Item *item, int slot);
        void unequip(int slot);
        void addItem(int key);
        int takeRandomItem();
        void takeItem(int i);
        bool canMove();
        template<typename T>
        static int byteHexStringToInt(T first,T second);
};
#endif /*PLAYER_H_*/