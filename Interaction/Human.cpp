#include "Human.h"

Human::Human(int floorNumber, const int key){
    this->charisma = floorNumber * (1-(strengthMap[key]));
    this->strength = floorNumber * strengthCharismaRatio;
}

bool Human::fight(Player &p){
    int combinedStrength = this->strength + p.getStrength();
    if(rand() % combinedStrength >= this->strength){
        return true;
    }else{
        addItem(p.takeRandomItem());
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
            p.addItem(spareItem);
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
        float itemValue = (1-strengthCharismaRatio) * (p.getLuck() / (this->strength + this->charisma)) * 5;//5 is the number of rarity classifications
        vector<int> possibleItems = Item::itemRarity[itemValue];
        vector<int>::iterator it;
        it = possibleItems.begin() + (int)(rand() % possibleItems.size());
        p.addItem(*it);
        this->addItem(i);
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
        addItem(p.takeRandomItem());
    
        return name + " persuaded you to give them an item.";
    }
}

dialogueOption Human::converse(dialogueOption d, int subOption){
    return d.nextOptions[subOption];
}