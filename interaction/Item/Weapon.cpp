#include "Weapon.h"

#define changeToWeaponStruct(a,b,c) {a + NUM_STD_ITEMS, weaponMod{.damage = b, .invSpace = c}}

const unordered_map<short,weaponMod> Weapon::weaponModMap = {
   changeToWeaponStruct(0,3,1),
   changeToWeaponStruct(1,1,1),
   changeToWeaponStruct(2,3,2)
};

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
    std::array<int,4> mods = {weaponMods.damage, 0, 0, weaponMods.invSpace};
    return mods;
}