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

array<int,4> Weapon::modifiers() {
    std::array<int,4> modsnt mods [4] = {weaponMods.damage, 0, 0, weaponMods.invSpace};
    return mods;
}