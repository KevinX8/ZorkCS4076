#ifndef HUMAN_H_
#define HUMAN_H_
#include <vector>
using namespace std;
#include "NPC.h"
#include "Player.h"
#include "..\GameWorld\Room.h"

class Human : public NPC {
    private:
        int strength;
        int charisma;
        double strengthCharismaRatio;
        string name;
        string usefulInfo;
        dialogueOption baseOption;
    public:
        Human(int FloorNumber, int key);
        ~Human();
        bool fight(Player &p);
        string askInfo(Player &p);
        string giveItem(int, Player &p);
        dialogueOption converse(dialogueOption, int subOption = -1);
        string spareOrKill(bool spare, Player &p);
};
#endif /*HUMAN_H_*/