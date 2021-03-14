#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#define NUM_STD_ITEMS 10
#define NUM_WEAPONS 10
#define TOTAL_ITEMS 30

using namespace std;

const unordered_map<short,string> itemNameMap = {
   {0, "key"},
   {1, "pen"},
   {2, "water bottle"},

   {0 + NUM_STD_ITEMS, "Big Massive Sword"},
   {1 + NUM_STD_ITEMS, "dagger"},

   /*anything below 50 is a standard item
   anything between 50 and 99 is a weapon
   anything between 100 and 150 is a wearable
   anything above 150 is a wearable weapon */
};

const vector<vector<short>> itemRarity = {
	//Hierachal items spawn twice as often as subsequent index
    /*
    index 0: item key never spawns
    index 1: common item which spawns in room/never spawns in locked room (8/15 items are in here)
    index 2: uncommon item which spawns in room/common item which spawns in locked room (4/15 items are in here)
    index 3: very rare item which spawns in room/uncommon item which spawns in locked room (2/15 items are in here)
    index 4: item only spawns in locked room (1/15 items are in here)
    */
   {0},{1,2},{50},{51},{}
};

class Item {
private:
	string description;
	short rarity;
public:
	Item (short itemHashcode);

	string getShortDescription();
	string getLongDescription();
	short getRarity();
	short hashCode;
	bool operator==(const int& key);
};

#endif /*ITEM_H_*/