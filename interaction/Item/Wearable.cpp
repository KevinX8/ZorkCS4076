
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