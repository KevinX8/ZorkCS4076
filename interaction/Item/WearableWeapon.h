#ifndef WEARABLEWEAPON_H_
#define WEARABLEWEAPON_H_
#include "Wearable.h"
#include "Weapon.h"

class WearableWeapon : public Wearable, public Weapon {
    private:

    public:
    WearableWeapon(const short hashkey);
};

#endif /*WEARABLEWEAPON_H_*/