#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include "item.h"
#include "Tools.h"

class Room {

private:
	string description;
	string exitString();
    vector<Item> itemsInRoom;
	unordered_set<int> cells;
	Coordinate coordinate;
public:
    int numberOfItems();
	Room(unordered_set <int> cells,string description);
	string shortDescription();
    void addItem(Item *inItem);
    void removeItemFromRoom(int location);
	Coordinate getCoordinate();
	bool cellInRoom(Coordinate c);
	unordered_set<int> getCells();
	operator Coordinate();
};

#endif
