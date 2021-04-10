#include "item.h"

const unordered_map<short,string> Item::itemNameMap = {
   {0, "key"},
   {1, "pen"},
   {2, "water bottle"},
   {3, "smashed glasses"},
   {4, "telecope"},
   {5, "scooby-snax"},
   {6, "ice cream"},
   {7, "laptop"},
   {8, "phone"},
   {9, "guitar"},
   {10, "hand sanitizer"},
   {11, "toothpaste"},
   //weapons
   {0 + NUM_STD_ITEMS, "Big Massive Sword"},
   {1 + NUM_STD_ITEMS, "dagger"},
    {2 + NUM_STD_ITEMS, "axe"},
    //wearables
    {0 + NUM_STD_ITEMS + NUM_WEAPONS,"Big boy pants"},
    {1 + NUM_STD_ITEMS + NUM_WEAPONS,"Bottle of Luck"},
    {2 + NUM_STD_ITEMS + NUM_WEAPONS,"Cool hat"},
    {3 + NUM_STD_ITEMS + NUM_WEAPONS,"face mask"},
    {4 + NUM_STD_ITEMS + NUM_WEAPONS,"rolex watch"},
    //wearable weapons
    {0 + NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES,"swagger dagger"},
    {1 + NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES,"wizards staff"}
};

const vector<vector<short>> Item::itemRarity = {
	//Hierachal items spawn twice as often as subsequent index
    /*
    index 0: item key never spawns
    index 1: common item which spawns in room/never spawns in locked room (8/15 items are in here)
    index 2: uncommon item which spawns in room/common item which spawns in locked room (4/15 items are in here)
    index 3: very rare item which spawns in room/uncommon item which spawns in locked room (2/15 items are in here)
    index 4: item only spawns in locked room (1/15 items are in here)
    */
   {0},{1,2,3,4,5,6,7,8,9,10,11},{0+NUM_STD_ITEMS},{1+NUM_STD_ITEMS,3 + NUM_STD_ITEMS + NUM_WEAPONS,2 + NUM_STD_ITEMS + NUM_WEAPONS},{2+NUM_STD_ITEMS,4 + NUM_STD_ITEMS + NUM_WEAPONS,0 + NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES,1 + NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES}
};

string Item::getType(int hashCode) {
    if (hashCode < NUM_STD_ITEMS) {
        return "Item: ";
    } else if (hashCode < NUM_WEAPONS + NUM_STD_ITEMS) {
        return "Weapon: ";
    } else if (hashCode < NUM_WEAPONS + NUM_STD_ITEMS + NUM_WEARABLES) {
        return "Wearable: ";
    } else {
        return "Wearable Weapon: ";
    }
    return "Null type: ";
}

Item::Item(const short key) {
    this->hashCode = key;
    this->description = itemNameMap.at(key);
    int index = 0;
    for (std::vector<std::vector<short>>::const_iterator rarityvectorit = itemRarity.begin(); rarityvectorit != itemRarity.end(); ++rarityvectorit) {
        if (std::find(rarityvectorit->begin(),rarityvectorit->end(),key) != rarityvectorit->end()) {
            this->rarity = index;
            break;
        }
        ++index;
    }
}

short Item::getRarity() {
    return rarity;
}

string Item::getShortDescription()
{
	return description;
}

array<int,4> Item::modifiers() {
    //strength, charisma, luck, invSpace
    array<int,4> mods = {0,0,0,0};
    return mods;
}

bool Item::operator==(const int& key)
{
    return this->hashCode == key;
}
