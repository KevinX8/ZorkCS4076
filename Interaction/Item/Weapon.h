#ifndef WEAPON_H_
#define WEAPON_H_
#include "item.h"

struct weaponMod {
    short damage;
    short invSpace;
};

class Weapon : public Item {
    private:
    weaponMod weaponMods;
    public:
    Weapon(const short hashkey);
    short getDamage();
    short getInvSpace();
    array<int,4> modifiers();
    static const unordered_map<short,weaponMod> weaponModMap;
};

#endif /*WEAPON_H_*/