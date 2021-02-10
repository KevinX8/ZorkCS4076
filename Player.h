#ifndef PLAYER_H_
#define PLAYER_H_
#include <vector>
#include <iostream>
#include "Item.h"
using namespace std;

class Player {
    private:
        vector<Item> inventory;

    public:
        double stealth, strength, charisma;

};
#endif /*PLAYER_H_*/