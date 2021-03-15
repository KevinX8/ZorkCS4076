#include "item.h"

Item::Item(const short key){
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

string Item::getLongDescription()
{
	return " item(s), " + description + ".\n";
}

array<int,4> Item::modifiers() {
    array<int,4> mods = {0,0,0,0};
    return mods;
}

bool Item::operator==(const int& key)
{
    return this->hashCode == key;
}