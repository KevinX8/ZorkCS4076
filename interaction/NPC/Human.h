#ifndef HUMAN_H_
#define HUMAN_H_
#include <vector>
#include <cmath>
using namespace std;
#include "NPC.h"
#include "../DialogueOption.h"

union strengthItem {
    int strength;
    int itemCode: 5;
};

class Human : public NPC {
    private:
        const DialogueOption<string> option1 = DialogueOption<string>("Question 1", "Answer 1", {DialogueOption<string>("question1A", "answer1A", {}),
                                                                                                 DialogueOption<string>("question1B", "answer1B", {}),
                                                                                                 DialogueOption<string>("question1C", "answer1C", {}),
                                                                                                 DialogueOption<string>("question1D", "answer1D", {}) } );
        const DialogueOption<string> option2 = DialogueOption<string>("Question 2", "Answer 2", {DialogueOption<string>("question2A", "answer2A", {}),
                                                                                                 DialogueOption<string>("question2B", "answer2B", {}),
                                                                                                 DialogueOption<string>("question2C", "answer2C", {}),
                                                                                                 DialogueOption<string>("question2D", "answer2D", {})});
        const DialogueOption<string> option3 = DialogueOption<string>("Question 3", "Answer 3", {DialogueOption<string>("question3A", "answer3A", {}),
                                                                                                 DialogueOption<string>("question3B", "answer3B", {}),
                                                                                                 DialogueOption<string>("question3C", "answer3C", {}),
                                                                                                 DialogueOption<string>("question3D", "answer3D", {})});
        const DialogueOption<string> option4 = DialogueOption<string>("Question 4", "Answer 4", {DialogueOption<string>("question4A", "answer4A", {}),
                                                                                                 DialogueOption<string>("question4B", "answer4B", {}),
                                                                                                 DialogueOption<string>("question4C", "answer4C", {}),
                                                                                                 DialogueOption<string>("question4D", "answer4D", {})});
        friend class NPC;
        int strength;
        int charisma;
        short key;
    public:
        const DialogueOption<string> baseOption;
        Human(int FloorNumber, const short key, bool inventoryEmpty = false);
        ~Human();
        bool fight(Player &p);
        int askInfo(Player &p);
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
        const static unordered_map<short,short> usefulInfoMap;
        const static unordered_map<short,double> strengthCharismaRatio;
        const static unordered_map<short,short> likeMap;
        vector<int> getInventory();
        string getName();
        string usefulInfo;
};
#endif /*HUMAN_H_*/
