#include "item.h"

const map<int,string> Item::itemNameMap = {
   {0, "key"},
   {1, "pen"},
   {2, "water bottle"},
   {3, "dagger"},
   {4, "Big Massive Sword"}
};

const vector<vector<int>> Item::itemRarity = {
    /*
    index 0: item key never spawns
    index 1: common item which spawns in room/never spawns in locked room
    index 2: uncommon item which spawns in room/common item which spawns in locked room
    index 3: very rare item which spawns in room/uncommon item which spawns in locked room
    index 4: item only spawns in locked room
    */
   {0},{1,2},{3},{4}
};

Item::Item (string inDescription, int inValue, short rarity) {
	description = inDescription;
	value = inValue;
    this->rarity = rarity;

}

Item::Item(string inDescription) {
	description = inDescription;
    value = 0;
    rarity = 0;
}

inline short Item::getRarity() {
    return rarity;
}
inline int Item::getValue() {
    return value;
}
/**void Item::setWeaponCheck(int isWeapon)
{
    if(isWeapon > 0 || isWeapon < 0)
        cout << "Item not a weapon" ;
    else
        cout << "Item is a weapon" ;
}*/

string Item::getShortDescription()
{
	return description;
}

string Item::getLongDescription()
{
	return " item(s), " + description + ".\n";
}

