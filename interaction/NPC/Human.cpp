#include "Human.h"

const unordered_map<short,double> Human::strengthCharismaRatio = {
   {0, 0.4},
   {1, 0.5},
   {2, 1},
   {3, 0.5},
   {4, 0.2},
   {5, 0.3},
   {6, 0.1}
};

const unordered_map<short,string> Human::usefulInfoMap = {
   {0, "Paulis Gributs likes "},
   {1, "Thomas Greaney likes "},
   {2, " likes "},
   {3, " likes "},
   {4, " likes "},
   {5, " likes "},
   {6, " likes "}
};

Human::Human(int floorNumber, const short key, bool inventoryEmpty): baseOption(DialogueOption<string>("", ("How can I help you?"), {option1, option2, option3, option4})){
    this->key = key;
    this->charisma = (floorNumber + STARTING_ATRRIBUTE_POINTS) * (1-(strengthCharismaRatio.at(key)));
    this->strength = (floorNumber + STARTING_ATRRIBUTE_POINTS) * strengthCharismaRatio.at(key);
    this->usefulInfo = usefulInfoMap.at(key);
    this->name = nameMap.at(key);
    if(!inventoryEmpty){
    }
}

bool Human::fight(Player &p){
    int combinedStrength = this->strength + p.getStrength();
    if(rand() % combinedStrength >= this->strength){
        return true;
    }else{
        int item = p.takeRandomItem();
        if(item >= 0){
            inventoryItems.push_back(item);
        }
        return false;
    }
} 

string Human::spareOrKill(bool spare, Player &p){
    if(spare){
        if(hasKey){
            p.addItem(0);
            hasKey = false;
            return "Thanks for your mercy. Here is this key I found.";
        }else{
            float itemValue = strengthCharismaRatio.at(key) * (p.getLuck() / (this->strength + this->charisma)) * Item::itemRarity.size();
            vector<short> possibleItems = Item::itemRarity[itemValue];
            vector<short>::iterator it;
            it = possibleItems.begin() + (int)(rand() % possibleItems.size());
            p.addItem(*it);
            return "Thanks for your mercy. Here is a token for you.";
        }
    }else{
        if(hasKey){
            p.addItem(0);
        }
        for(int i : inventoryItems){
            p.addItem(i);
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
            return "I love these. Thank you!";
        }
    }else{
        return "I hate these. You can keep it.";
    }
}

string Human::askInfo(Player &p){
    int combinedCharisma = p.getCharisma() + this->charisma;
    if(rand() % combinedCharisma >= this->charisma){
        return usefulInfo;
    }else{
        int item = p.takeRandomItem();
        if(item >= 0){
            inventoryItems.push_back(p.takeRandomItem());
        }
    
        if(item >= 0){
            return "You failed to persuade " + name + ". " + name + " persuaded you to give them an item!";
        }else{
            return "You failed to persuade " + name + ".";
        }
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
