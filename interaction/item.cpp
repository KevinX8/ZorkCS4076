#include "item.h"

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

