#ifndef PLAYER_H_
#define PLAYER_H_
#include <vector>
#include <iostream>
#include "Item.h"
using namespace std;

class NPC {
    private:
        vector<Item> inventory;
    public:
        getDialogue();
        addItem(int key);

};
#endif /*PLAYER_H_*/