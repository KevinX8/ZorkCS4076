#include "Human.h"

Human::Human(int floorNumber, const short key){
    this->key = key;
    this->charisma = (floorNumber + STARTING_ATRRIBUTE_POINTS) * (1-(strengthCharismaRatio.at(key)));
    this->strength = (floorNumber + STARTING_ATRRIBUTE_POINTS) * strengthCharismaRatio.at(key);
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
            float itemValue = strengthCharismaRatio.at(key) * (p.getLuck() / (this->strength + this->charisma)) * itemRarity.size();
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
        float itemValue = (1-strengthCharismaRatio.at(key)) * (p.getLuck() / (this->strength + this->charisma)) * itemRarity.size();
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

void NPC::giveKey(){
    hasKey = true;
}

DialogueOption<string> Human::converse(DialogueOption<string> d, int subOption){
    return d.getNextOption().at(subOption);
}

int NPC::getLikedItem(){
    return likedItem;
}

Human::~Human(){

}
