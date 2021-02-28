#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include "..\Interaction\item.h"
#include "Tools.h"

struct Door { //replace with union maybe
    Coordinate doorLocation;
    bool vertical;
	bool locked;
    int roomKey1;
    int roomKey2;
};
class Room {

private:
	string description;
	string exitString();
    vector<Item> itemsInRoom;
	vector<Door> doorsInRoom;
	unordered_set<int> cells;
	void addDoor(Door door);
    int key;
	friend class Floor;
public:
    int numberOfItems();
	Room(unordered_set <int> cells,string description);
	string shortDescription();
    void addItem(Item *inItem);
    void removeItemFromRoom(int location);
	int getKey();
	bool cellInRoom(Coordinate c);
	unordered_set<int> getCells();
	vector<Door> getDoors();
	operator int();
	bool operator<(Room r2);
};

#endif
