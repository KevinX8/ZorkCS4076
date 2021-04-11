#ifndef NPC_H_
#define NPC_H_
#include <vector>
#include <iostream>
#include <unordered_map>
#include "../Player.h"
#include "../../QTGUI/InventoryWidget.h"

using namespace std;

#define NUM_HUMANS 15
#define NUM_NPCS 15

const unordered_map<short,string> nameMap = {
   {0, "Thomas Greaney"},
   {1, "Paulis Gributs"},
   {2, "The Rock"},
   {3, "Gandalf"},
   {4, "Leonardo di Caprio"},
   {5, "Shaggy"},
   {6, "Santa Claus"},
   {7, "Chris Exton"},
   {8, "Oprah"},
   {9, "Michael D. Higgins"},
   {10, "Random Loser"},
   {11, "Snoop Dog"},
   {12, "Bill Burr"},
   {13, "Joe Rogan"},
    {14, "Nicki Minaj"},
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
        virtual int giveItem(int giftItem,Player &p, InventoryWidget* inv)  = 0;
        virtual void giveKey() = 0;
        virtual int getLikedItem() = 0;
        virtual void addItem(int code) = 0;
        virtual int getCode() = 0;
        virtual vector<int> getInventory() = 0;
        virtual string getName() = 0;
        virtual string spareOrKill(bool spare, Player &p, InventoryWidget* inv) = 0;
        virtual int fight(Player &p) = 0;
        virtual int askInfo(Player &p) = 0;
        /*
        static const map<int,float> strengthMap;
        static const map<int,int> giftItemMap;
        static const map<int,int> likedItemMap;
        */
};
#endif /*NPC_H_*/
