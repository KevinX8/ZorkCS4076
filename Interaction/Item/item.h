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
#define NUM_WEAPONS 10
#define TOTAL_ITEMS 30

using namespace std;

class Item {
private:
	string description;
	short rarity;
public:
	Item (short itemHashcode);

	string getShortDescription();
	string getLongDescription();
	short getRarity();
   virtual array<int,4> modifiers();
	short hashCode;
	bool operator==(const int& key);
   static const vector<vector<short>> itemRarity;
   static const unordered_map<short,string> itemNameMap;
};

#endif /*ITEM_H_*/