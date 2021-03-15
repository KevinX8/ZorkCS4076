#include "Player.h"

#define space 20
#define NEXT_HEX byteHexStringToInt(nextChar(playerToken, offset),nextChar(playerToken, offset))

Player::Player(){
    this->inventorySpace = space;
    this->inventory = vector<Item>();
}

Player::Player(string playerToken){
    int *offset = 0;
    for (int item=0; item < NEXT_HEX; item++) {
        inventory.push_back(Item(NEXT_HEX));
    }
    this->equip(new Item(NEXT_HEX), 0);
    this->equip(new Item(NEXT_HEX), 1);
    this->equip(new Item(NEXT_HEX), 2);
}

void Player::changeInventorySpace(int amount){
    inventorySpace += amount;
}

void Player::changeLuck(int amount){
    luck += amount;
}

void Player::changeStrength(int amount){
    strength += amount;
}

void Player::changeCharisma(int amount){
    charisma += amount;
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
    inventory.push_back(Item(item));
}

int Player::takeRandomItem(){
    vector<Item>::iterator it;
    it = inventory.begin() + (int)(rand() % inventory.size());
    int item = (*it).hashCode;
    inventory.erase(it);
    return item;
}

void Player::takeItem(int item){    
    for(auto it = inventory.begin(); it != inventory.end(); ++it){
        if((*it).hashCode == item){
            inventory.erase(it);
            return;
        }
    }
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

bool Player::equip(Item *item, int slot){

/*
0: Strength Modifier
1: Charisma Modifier
2: Luck Modifier
3: Inventory Space Modifier
*/
    std::array<int,4> modifiers;
    switch(slot){
        case(0):{
            if(Weapon *weapon = dynamic_cast<Weapon*>(item)){
                if (!activeWeapon) {
                    activeWeapon = weapon;
                    copy(modifiers.begin(), modifiers.end(),weapon->modifiers().begin());
                }
            }
            break;
        }
        case(1):{
            if(Wearable* wearable = dynamic_cast<Wearable*>(item)){
                if (activeWearable1 == 0) {
                    activeWearable1 = wearable;
                    copy(modifiers.begin(), modifiers.end(),wearable->modifiers().begin());
                }
            }
            break;
        }
        case(2):{
            if(Wearable* wearable = dynamic_cast<Wearable*>(item)){
                if (activeWearable2 == 0) {
                    activeWearable2 = wearable;
                    copy(modifiers.begin(), modifiers.end(),wearable->modifiers().begin());
                }
            }
            break;
        }
    }
}
