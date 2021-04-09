#include "Player.h"

#define space 20
#define NEXT_HEX byteHexStringToInt(Tools::nextChar(playerToken, offset),Tools::nextChar(playerToken, offset))

template<typename T>
int Player::byteHexStringToInt(T first,T second) {
    stringstream ss;
    string number = "";
    ss << first;
    ss << second;
    ss << std::hex << ss.str();
    ss >> number;
    return stoi(number);
}

Player::Player(){
    this->inventorySpace = space;
    this->inventory = vector<shared_ptr<Item>>();
    this->luck = 2;
    this->strength = 1;
    this->charisma = 1;
}

Player::Player(string playerToken){
    int *offset = 0;
    for (int item=0; item < NEXT_HEX; item++) {
        inventory.push_back(shared_ptr<Item>(new Item(NEXT_HEX)));
    }
    this->equip(shared_ptr<Item>(new Item(NEXT_HEX)), 0);
    this->equip(shared_ptr<Item>(new Item(NEXT_HEX)), 1);
    this->equip(shared_ptr<Item>(new Item(NEXT_HEX)), 2);
}

int Player::getLuck(){
    if(luck < 0){
        return 0;
    }
    return luck;
    
}

int Player::getStrength(){
    if(strength < 0){
        return 0;
    }
    return strength;
}

int Player::getCharisma(){
    if(charisma < 0){
        return 0;
    }
    return charisma;
}

void Player::addItem(int item){
    inventory.push_back(shared_ptr<Item>(new Item(item)));
}

int Player::takeRandomItem(){
    if(inventory.size() > 0){
        auto it = inventory.begin() + (int)(rand() % inventory.size());
        int item = (*it)->hashCode;
        inventory.erase(it);
        return item;
    }else{
        return -1;
    }
}

int Player::takeItem(int item){
    int index = 0;
    for(auto it = inventory.begin(); it != inventory.end(); ++it){
        if((*it)->hashCode == item){
            inventory.erase(it);
            return index;
        }
        index++;
    }
    return -1;
}

void Player::changeParams(int params[], bool equip) {
    if(equip){
        strength += params[0];
        charisma += params[1];
        luck += params[2];
        inventorySpace += (int)(params[3]); //6 is the most you can minus by
    } else {
        strength -= params[0];
        charisma -= params[1];
        luck -= params[2];
        inventorySpace -= (int)(params[3]);  
    }
}

bool Player::canMove(){
    return inventory.size() <= inventorySpace;
}

bool Player::equip(shared_ptr<Item> item, int slot){

/*
0: Strength Modifier
1: Charisma Modifier
2: Luck Modifier
3: Inventory Space Modifier
*/
    std::array<int,4> modifiers;
    switch(slot){
        case(0):{
            if(item->hashCode >= NUM_STD_ITEMS){
                if (!activeWeapon) {
                    shared_ptr<Weapon> weapon = shared_ptr<Weapon>(new Weapon(item->hashCode));
                    activeWeapon = weapon;
                    copy(modifiers.begin(), modifiers.end(),weapon->modifiers().begin());
                }
            }
            return true;
        }
        case(1):{
            if(item->hashCode >= NUM_STD_ITEMS+ NUM_WEAPONS){
                auto wearable = shared_ptr<Wearable>(new Wearable(item->hashCode));
                if (!activeWearable1) {
                    activeWearable1 = wearable;
                    copy(modifiers.begin(), modifiers.end(),wearable->modifiers().begin());
                } else if (!activeWearable2) {
                    activeWearable2 = wearable;
                    copy(modifiers.begin(), modifiers.end(),wearable->modifiers().begin());
                } else {
                    return false;
                }
            }
            return true;
        }
        case(2):{
            if(item->hashCode >= NUM_STD_ITEMS+ NUM_WEAPONS){
                auto wearable = shared_ptr<Wearable>(new Wearable(item->hashCode));
                if (!activeWearable2) {
                    activeWearable2 = wearable;
                    copy(modifiers.begin(), modifiers.end(),wearable->modifiers().begin());
                } else {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void Player::unequip(int slot) 
{
    switch (slot) {
    case 0: addItem(activeWeapon->hashCode); activeWeapon.reset();break;
    case 1: addItem(activeWearable1->hashCode);activeWearable1.reset();break;
    case 2: addItem(activeWearable2->hashCode);activeWearable2.reset(); break;
    }
}
