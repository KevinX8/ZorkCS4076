#ifndef WEARABLE_H_
#define WEARABLE_H_
#include "item.h"

#define changeToWearableStruct(a,b,c,d) {a + NUM_STD_ITEMS + NUM_WEAPONS, wearableMod{.charisma = b, .luck = c, .invSpace = d}}

struct wearableMod {
    short charisma;
    short luck;
    short invSpace;
};

const unordered_map<short,wearableMod> wearableModMap = {
    /*charisma luck invSpace */
   changeToWearableStruct(0, 0, 3, 0), 
   changeToWearableStruct(1, 3, 0 ,0)
};

class Wearable : public Item {
    private:
    wearableMod wearableMods;
    public:
    Wearable(const short hashkey);
    short getCharisma();
    short getLuck();
    short getInvSpace();
    array<int,4> modifiers();
};

#endif /*WEARABLE_H_*/