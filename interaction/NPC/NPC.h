#ifndef NPC_H_
#define NPC_H_
#include <vector>
#include <iostream>
#include <unordered_map>
#include "../Player.h"

using namespace std;

#define NUM_HUMANS 7
#define NUM_NPCS 7

const unordered_map<short,string> nameMap = {
   {0, "Thomas Greaney"},
   {1, "Paulis Gributs"},
   {2, "The Rock"},
   {3, "Gandalf"},
   {4, "Leonardo di Caprio"},
   {5, "Shaggy"},
   {6, "Santa Claus"},

    {0 + NUM_HUMANS, "Scooby-Doo"},
    {1 + NUM_HUMANS, "Garfield"},
    {2 + NUM_HUMANS, "Mufasa"},
    {3 + NUM_HUMANS, "Thumper"},
    {4 + NUM_HUMANS, "Rudolf"}
   /*
   anything below 50 is a human
   anything between 50 and 99 is an animal
   */
};

class NPC {
    protected:
        string name;
        bool hasKey;
        int likedItem;
        vector<int> inventoryItems;
    public:
        virtual int getSpareItem() = 0;
        virtual string giveItem(int giftItem,Player &p)  = 0;
        virtual void giveKey() = 0;
        virtual int getLikedItem() = 0;
        virtual void addItem(int code) = 0;
        virtual int getCode() = 0;
        virtual vector<int> getInventory() = 0;
        /*
        static const map<int,float> strengthMap;
        static const map<int,int> giftItemMap;
        static const map<int,int> likedItemMap;
        */
};
#endif /*NPC_H_*/
