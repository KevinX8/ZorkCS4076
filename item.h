#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>
#include <iostream>
using namespace std;

class Item {
private:
	string description;
	string longDescription;
	short rarity;
	int value;
public:
    Item (string description, int inValue, short rarity);
    Item (string description);
	string getShortDescription();
    string getLongDescription();
	short getRarity();
	int getValue();
};

#endif /*ITEM_H_*/