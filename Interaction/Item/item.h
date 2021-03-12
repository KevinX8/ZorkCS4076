#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Item {
private:
	string description;
	string longDescription;
	short rarity;
public:
	Item (short itemHashcode);
	string getShortDescription();
    string getLongDescription();
	short getRarity();
	int getValue();
	const short hashCode;
	static const map<int,string> itemNameMap;
	static const vector<vector<int>> itemRarity;
};

#endif /*ITEM_H_*/