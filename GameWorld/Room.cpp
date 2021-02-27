#include "Room.h"

Room::Room(unordered_set<int> cells,string description = "This is just a room") {
    this->key = 9999999;
	for(int cellKey : cells){
        this->key = min(cellKey, key);
	}
	this->cells = cells;
	this->description = description;
}

string Room::shortDescription() {
	return description;
}

void Room::addItem(Item *inItem) {
    //cout <<endl;
    //cout << "Just added" + inItem->getLongDescription();
    itemsInRoom.push_back(*inItem);
}

int Room::numberOfItems() {
    return itemsInRoom.size();
}

bool Room::cellInRoom(Coordinate c){
	return cells.find(Tools::getCoordinateKey(c)) != cells.end();
}

int Room::getKey(){
	return key;
}

unordered_set<int> Room::getCells(){
	return cells;
}

vector<Door> Room::getDoors(){
	return doorsInRoom;
}

void Room::addDoor(Door door){
	doorsInRoom.push_back(door);
}

Room::operator int() {
	return key;
}

bool Room::operator<(Room r2){
	return r2.getKey() >= this->getKey();
}