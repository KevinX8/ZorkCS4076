#include "Human.h"

const unordered_map<short,double> Human::strengthCharismaRatio = {
   {0, 0.4},
   {1, 0.5},
   {2, 1},
   {3, 0.5},
   {4, 0.2},
   {5, 0.3},
   {6, 0.1},
   {7, 0.5},
   {8, 0.2},
   {9, 0.3},
   {10, 0.8},
   {11, 0.4},
   {12, 0.2},
   {13, 0.6},
   {14,0}
};

const unordered_map<short,short> Human::usefulInfoMap = {
   {0, 1},
   {1, 0},
   {2, 13},
   {3, 6},
   {4, 8},
   {5, 14},
   {6, 7},
   {7, 10},
   {8, 4},
   {9, 3},
   {10, 9},
   {11, 5},
   {12, 2},
   {13, 12},
   {14, 11}
};

const unordered_map<short,short> Human::likeMap = {
   {0, 9},
   {1, 0 + NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES},
   {2, 10},
   {3, 1 + NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES},
   {4, 4},
   {5, 5},
   {6, 0 + NUM_STD_ITEMS + NUM_WEAPONS},
   {7, 7},
   {8, 2 + NUM_STD_ITEMS + NUM_WEAPONS},
   {9, 3},
   {10, 1},
   {11, 0 + NUM_STD_ITEMS},
   {12, 4 + NUM_STD_ITEMS + NUM_WEAPONS},
   {13, 3 + NUM_STD_ITEMS + NUM_WEAPONS},
   {14, 11}
};

Human::Human(int floorNumber, const short k, bool inventoryEmpty): baseOption(DialogueOption<string>("", ("How can I help you?"), {option1, option2, option3, option4})){
    this->key = k;
    this->hasKey = false;
    this->charisma = (floorNumber + STARTING_ATRRIBUTE_POINTS) * (1-(strengthCharismaRatio.at(key)));
    this->strength = (floorNumber + STARTING_ATRRIBUTE_POINTS) * strengthCharismaRatio.at(key);
    this->usefulInfo = nameMap.at(usefulInfoMap.at(key)) + " likes " + Item::itemNameMap.at(likeMap.at(usefulInfoMap.at(key)));
    this->name = nameMap.at(key);
    this->likedItem = likeMap.at(key);
    if(!inventoryEmpty){
    }
}

int Human::fight(Player &p){
    strengthItem si;
    si.strength = this->strength + p.getStrength();
    if(rand() % si.strength >= this->strength){
        return -1;
    }else{
        Coordinate c = p.takeRandomItem();
        si.itemCode = c.x;
        if(si.itemCode >= 0){
            inventoryItems.push_back(si.itemCode);
        }
        return c.y;
    }
} 

string Human::spareOrKill(bool spare, Player &p, InventoryWidget* inv){
    if(spare){
        if(hasKey){
            p.addItem(0);
            hasKey = false;
            return "Thanks for your mercy. Here is this key I found.";
            inv->updateInventory(p.inventory.size()-1);
            inv->updateStats();
        }else{
            float itemValue = strengthCharismaRatio.at(key) * (p.getLuck() / (this->strength + this->charisma)) * (Item::itemRarity.size()-1) + 1;
            vector<short> possibleItems = Item::itemRarity[itemValue];
            vector<short>::iterator it;
            it = possibleItems.begin() + (int)(rand() % possibleItems.size());
            p.addItem(*it);
            inv->updateInventory(p.inventory.size()-1);
            inv->updateStats();
            return "Thanks for your mercy. Here is a token for you.";
        }
    }else{
        if(hasKey){
            p.addItem(0);
            inv->updateInventory(p.inventory.size()-1);
            inv->updateStats();
        }
        for(int i : inventoryItems){
            p.addItem(i);
            inv->updateInventory(p.inventory.size()-1);
            inv->updateStats();
        }
        return name + " is dead. You took all their items.";
    }
}

string Human::giveItem(int i, Player &p){
    if(i == likedItem){
        if(hasKey){
            p.addItem(0);
            hasKey = false;
            return "I love these. Here's a key I found.";
        }else{
            float itemValue = (1-strengthCharismaRatio.at(key)) * (p.getLuck() / (this->strength + this->charisma)) * Item::itemRarity.size();
            vector<short> possibleItems = Item::itemRarity[itemValue];
            vector<short>::iterator it;
            it = possibleItems.begin() + (short)(rand() % possibleItems.size());
            p.addItem(*it);
            inventoryItems.push_back(i);
            p.takeItem(i);
            return "I love these. Thank you! Here is a token of appreciation.";
        }
    }else{
        return "I hate these. You can keep it.";
    }
}

int Human::askInfo(Player &p){
    int combinedCharisma = p.getCharisma() + this->charisma;
    if(rand() % combinedCharisma >= this->charisma){
        return -2;
    }else{
        Coordinate result = p.takeRandomItem();
        if(result.x >= 0){
            inventoryItems.push_back(result.x);
        }
        return result.y;
    }
}

void Human::giveKey(){
    hasKey = true;
}

void Human::addItem(int code){
    inventoryItems.push_back(code);
}

DialogueOption<string> Human::converse(DialogueOption<string> d, int subOption){
    return d.getNextOption().at(subOption);
}

int Human::getLikedItem(){
    return likedItem;
}

int Human::getSpareItem(){
    return 0;
}

int Human::getCode(){
    return (int)(key);
}

int Human::getStrength(){
    return strength;
}

int Human::getCharisma(){
    return charisma;
}

vector<int> Human::getInventory(){
    return inventoryItems;
}

string Human::getName(){
    return name;
}

Human::~Human(){

}
