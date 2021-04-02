#ifndef HUMAN_H_
#define HUMAN_H_
#include <vector>
using namespace std;
#include "NPC.h"
#include "../DialogueOption.h"

class Human : public NPC {
    private:
        friend class NPC;
        int strength;
        int charisma;
        string usefulInfo;
        DialogueOption<string> baseOption;
        short key;
    public:
        Human(int FloorNumber, const short key, bool inventoryEmpty = false);
        ~Human();
        bool fight(Player &p);
        string askInfo(Player &p);
        DialogueOption<string> converse(DialogueOption<string>, int subOption = -1);
        string spareOrKill(bool spare, Player &p);
        int getSpareItem();
        string giveItem(int giftItem,Player &p);
        void giveKey();
        int getLikedItem();
        void addItem(int code);
        int getCode();
        int getStrength();
        int getCharisma();
        const static unordered_map<short,string> usefulInfoMap;
        const static unordered_map<short,double> strengthCharismaRatio;
        vector<int> getInventory();
        string getName();
};
#endif /*HUMAN_H_*/
