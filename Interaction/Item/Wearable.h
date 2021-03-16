#ifndef WEARABLE_H_
#define WEARABLE_H_
#include "item.h"

struct wearableMod {
    short charisma;
    short luck;
    short invSpace;
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
    static const unordered_map<short,wearableMod> wearableModMap;
};

#endif /*WEARABLE_H_*/