#include "Weapon.h"

Weapon::Weapon(const short key) : Item(key) {
    this->weaponMods = weaponModMap.at(key);
}

short Weapon::getDamage() {
    return weaponMods.damage;
}

short Weapon::getInvSpace() {
    return weaponMods.invSpace;
}