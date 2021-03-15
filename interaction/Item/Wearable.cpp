
#include "Wearable.h"

Wearable::Wearable(const short key) : Item(key) {
    this->wearableMods = wearableModMap.at(key);
}

short Wearable::getCharisma() {
    return wearableMods.charisma;
}

short Wearable::getInvSpace() {
    return wearableMods.invSpace;
}

array<int,4> Wearable::modifiers() {
    std::array<int,4> mods = {0, wearableMods.charisma, 0, wearableMods.invSpace};
    return mods;
}