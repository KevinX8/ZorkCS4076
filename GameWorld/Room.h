#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include "..\Interaction\Item\item.h"
#include "..\Interaction\NPC\Human.h"
#include "Tools.h"

class Room;

struct Door {
    Coordinate doorLocation;
    bool vertical;
	bool locked;
    int roomIndex;
	Door(const Door& d) {
		doorLocation = d.doorLocation;
		vertical = d.vertical;
		locked = d.locked;
		roomIndex = d.roomIndex;
	}
	Door(){
		
	}
};

class Room {

private:
	friend class Floor;
    vector<int> itemsInRoom;
    vector<shared_ptr<NPC>> npcsInRoom;
    vector<Door> doorsInRoom;
	unordered_set<int> cells;
    void addDoor(Door& door);
    int key;
	bool hasUpLadder;
	bool hasDownLadder;
	bool hasKiosk;
public:
    int numberOfItems();
    bool containsUpLadder();
    bool containsDownLadder();
	Room(unordered_set <int> cells = {});
    void addItem(int i);
    void removeItemFromRoom(int location);
	int* upgradeStats(short statToUpgrade);
	bool getKiosk();
	int getKey();
	bool cellInRoom(Coordinate c);
	unordered_set<int> getCells();
	vector<int> getItems();
	vector<Door> &getDoors();
	operator int();
	bool operator<(Room r2);
	bool operator==(Room * const r2);
    shared_ptr<NPC> addNPC(int key, int floorNumber, bool emptyNPC = false);
    vector<shared_ptr<NPC>> &getNPCs();
	void giveLadder(bool up);
};

#endif
