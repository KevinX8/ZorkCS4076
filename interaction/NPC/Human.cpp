#include "Human.h"

Human::Human(int floorNumber, const int key){
    this->charisma = (floorNumber + STARTING_ATRRIBUTE_POINTS) * (1-strengthCharismaRatio[key]);
    this->strength = (floorNumber + STARTING_ATRRIBUTE_POINTS) * strengthCharismaRatio[key];
    this->usefulInfo = usefulInfo[key];
}

bool Human::fight(Player &p){
    int combinedStrength = this->strength + p.getStrength();
    if(rand() % combinedStrength >= this->strength){
        return true;
    }else{
        inventoryItems.push_back(p.takeRandomItem());
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
            float itemValue = (strengthCharismaRatio) * (p.getLuck() / (this->strength + this->charisma)) * Item::itemRarity.size();
            vector<short> possibleItems = itemRarity[itemValue];
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
        return name + " is dead.";
    }
}

string Human::giveItem(int i, Player &p){
    if(i == likedItem){
        float itemValue = (1-strengthCharismaRatio) * (p.getLuck() / (this->strength + this->charisma)) * Item::itemRarity.size();
        vector<short> possibleItems = itemRarity[itemValue];
        vector<short>::iterator it;
        it = possibleItems.begin() + (short)(rand() % possibleItems.size());
        p.addItem(*it);
        inventoryItems.push_back(i);
         p.takeItem(i);
        return "I love these.";
    }else{
        return "I hate these.";
    }
}

string Human::askInfo(Player &p){
    int combinedCharisma = p.getCharisma() + this->charisma;
    if(rand() % combinedCharisma >= this->charisma){
        return usefulInfo;
    }else{
        inventoryItems.push_back(p.takeRandomItem());
    
        return name + " persuaded you to give them an item.";
    }
}

void giveKey(){
    hasKey = true;
}

dialogueOption Human::converse(dialogueOption d, int subOption){
    return d.nextOptions[subOption];
}