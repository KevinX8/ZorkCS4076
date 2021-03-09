#ifndef PLAYER_H_
#define PLAYER_H_
#include <vector>
#include <iostream>
#include "Item.h"
using namespace std;

class Player {
    private:
        vector<Item> inventory;
        int inventorySpace;
    public:
        int luck, strength, charisma;
        void changeInventorySpace(int);
        void changeLuck(int);
        void changeStrength(int);
        void changeCharisma(int);
        void addItem();
        int getStrength();
        int getLuck();
        int getCharisma();
        void addItem(int key);
        int takeRandomItem();
        int takeItem(int i);

};
#endif /*PLAYER_H_*/