
#include "Wearable.h"
#define changeToWearableStruct(a,b,c,d) {a + NUM_STD_ITEMS + NUM_WEAPONS, wearableMod{.charisma = b, .luck = c, .invSpace = d}}

const unordered_map<short,wearableMod> Wearable::wearableModMap = {
    /*charisma luck invSpace */
   changeToWearableStruct(0, 0, 3, 0), 
   changeToWearableStruct(1, 3, 0 ,0)
};

Wearable::Wearable(const short key) : Item(key) {
    this->wearableMods = wearableModMap.at(key);
}

short Wearable::getCharisma() {
    return wearableMods.charisma;
}

short Wearable::getInvSpace() {
    return wearableMods.invSpace;
}

short Wearable::getLuck() {
    return wearableMods.luck;
}

array<int,4> Wearable::modifiers() {
    std::array<int,4> mods = {0, wearableMods.charisma, 0, wearableMods.invSpace};
    return mods;
}
