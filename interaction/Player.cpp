#include "Player.h"

#define space 20

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
    vector<int> hexOut;
    for (auto it = playerToken.begin(); it != playerToken.end();it+=2) {
        hexOut.insert(hexOut.begin(),byteHexStringToInt(*it,*(it+1)));
    }
    int noItems = hexOut.back();
    hexOut.pop_back();
    for (int item=0; item < noItems; item++) {
        inventory.push_back(shared_ptr<Item>(new Item(hexOut.back())));
        hexOut.pop_back();
    }
    this->equip(shared_ptr<Item>(new Item(hexOut.back())), 0);
    hexOut.pop_back();
    this->equip(shared_ptr<Item>(new Item(hexOut.back())), 1);
    hexOut.pop_back();
    this->equip(shared_ptr<Item>(new Item(hexOut.back())), 2);
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
    if(inventory.size() < inventorySpace){
        inventory.push_back(shared_ptr<Item>(new Item(item)));
    }
}

Coordinate Player::takeRandomItem(){
    Coordinate c;
    c.x = -1;
    c.y = -1;
    if(inventory.size() > 0){
        int index = (int)(rand() % inventory.size());
        auto it = inventory.begin() + index;
        int item = (*it)->hashCode;
        inventory.erase(it);
        c.x = item;
        c.y = index;
    }
    return c;
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

void Player::changeParams(std::array<int,4> params, bool equip) {
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
                    modifiers = weapon->modifiers();
                }
            }
            changeParams(modifiers,true);
            return true;
        }
        case(1):{
            if(item->hashCode >= NUM_STD_ITEMS+ NUM_WEAPONS){
                auto wearable = shared_ptr<Wearable>(new Wearable(item->hashCode));
                if (!activeWearable1) {
                    activeWearable1 = wearable;
                    modifiers = wearable->modifiers();
                } else if (!activeWearable2) {
                    activeWearable2 = wearable;
                    modifiers = wearable->modifiers();
                } else {
                    return false;
                }
            }
            changeParams(modifiers,true);
            return true;
        }
        case(2):{
            if(item->hashCode >= NUM_STD_ITEMS+ NUM_WEAPONS){
                auto wearable = shared_ptr<Wearable>(new Wearable(item->hashCode));
                if (!activeWearable2) {
                    activeWearable2 = wearable;
                    modifiers = wearable->modifiers();
                } else {
                    return false;
                }
            }
            changeParams(modifiers,true);
            return true;
        }
    }
    return false;
}

void Player::unequip(int slot) 
{
    std::array<int,4> modifiers;
    switch (slot) {
    case 0: addItem(activeWeapon->hashCode); modifiers = activeWeapon->modifiers(); activeWeapon.reset();break;
    case 1: addItem(activeWearable1->hashCode); modifiers = activeWearable1->modifiers(); activeWearable1.reset();break;
    case 2: addItem(activeWearable2->hashCode); modifiers = activeWearable2->modifiers(); activeWearable2.reset(); break;
    }
    changeParams(modifiers,false);
}
