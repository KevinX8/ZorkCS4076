#ifndef ITEM_H_
#define ITEM_H_

#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#define NUM_STD_ITEMS 3
#define NUM_WEAPONS 3
#define NUM_WEARABLES 1
#define NUM_WEARABLEWEAPONS 1
#define TOTAL_ITEMS NUM_STD_ITEMS + NUM_WEAPONS + NUM_WEARABLES + NUM_WEARABLEWEAPONS

using namespace std;

class Item {
private:
	string description;
	short rarity;
public:
	Item (short itemHashcode);

	string getShortDescription();
	short getRarity();
   virtual array<int,4> modifiers();
	short hashCode;
	bool operator==(const int& key);
   static const vector<vector<short>> itemRarity;
   static const unordered_map<short,string> itemNameMap;
   static string getType(int hashCode);
};

#endif /*ITEM_H_*/
