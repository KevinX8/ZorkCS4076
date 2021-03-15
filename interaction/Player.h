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

#define STARTING_ATRRIBUTE_POINTS 4

using namespace std;

template<typename T>
inline int byteHexStringToInt(T first,T second) {
    stringstream ss;
    string number = "";
    ss << first;
    ss << second;
    ss << std::hex << ss.str();
    ss >> number;
    return stoi(number);
}

inline char nextChar(string s, int *i) {
    char val = s.at(*i);
    ++*i;
    return val;
}

class Player {
    private:
        vector<Item> inventory;
        int inventorySpace;
        Weapon *activeWeapon;
        Wearable *activeWearable1;
        Wearable *activeWearable2;
        int luck, strength, charisma;
        void changeLuck(int);
        void changeStrength(int);
        void changeCharisma(int);
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
};
#endif /*PLAYER_H_*/