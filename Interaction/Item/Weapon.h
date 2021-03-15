#ifndef WEAPON_H_
#define WEAPON_H_
#include "item.h"

#define changeToWeaponStruct(a,b,c) {a + NUM_STD_ITEMS, weaponMod{.damage = b, .invSpace = c}}

struct weaponMod {
    short damage;
    short invSpace;
};

const unordered_map<short,weaponMod> weaponModMap = {
   changeToWeaponStruct(0,3,1),
   changeToWeaponStruct(1,1,1)
};

class Weapon : public Item {
    private:
    weaponMod weaponMods;
    public:
    Weapon(const short hashkey);
    short getDamage();
    short getInvSpace();
    array<int,4> modifiers();
};

#endif /*WEAPON_H_*/