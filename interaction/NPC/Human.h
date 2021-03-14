#ifndef HUMAN_H_
#define HUMAN_H_
#include <vector>
using namespace std;
#include "NPC.h"
#include "Player.h"

const unordered_map<short,double> strengthCharismaRatio = {
   {0, 0.4},
   {1, 0.5},
   {2, 1},
   {3, 0.5},
   {4, 0.2},
   {5, 0.3},
   {6, 0.1}
};

const unordered_map<short,double> usefulInfo = {
   {0, "Paulis Gributs likes "},
   {1, "Thomas Greaney likes "},
   {2, " likes "},
   {3, " likes "},
   {4, " likes "},
   {5, " likes "},
   {6, " likes "}
};

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