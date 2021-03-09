#ifndef NPC_H_
#define NPC_H_
#include <vector>
#include <iostream>
using namespace std;

struct dialogueOption{
    string text;
    string reply;
    vector<dialogueOption> nextOptions;
};

class NPC {
    protected:
        bool hasKey;
        int likedItem;
        vector<int> inventoryItems;
    public:
        NPC();
        vector<int> getKillItems();
        int getSpareItem();
        int giveItem(int giftItem);
        static const map<int,float> strengthMap;
        static const map<int,int> giftItemMap;
        static const map<int,int> likedItemMap;
};
#endif /*NPC_H_*/