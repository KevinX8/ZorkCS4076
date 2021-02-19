#include "Room.h"

Room::Room(unordered_set<int> cells,string description = "This is just a room") {
	Coordinate foo;
	foo.x = 0;
	foo.y = 0;
	int count = 0;
	for(int cellKey : cells){
		Coordinate cell = Tools::getKeyCoordinate(cellKey);
		foo.x += cell.x;
		foo.y += cell.y;
		count++;
	}
	foo.x /= count;
	foo.y /= count;
	this->coordinate = foo;
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

Coordinate Room::getCoordinate(){
	return coordinate;
}

unordered_set<int> Room::getCells(){
	return cells;
}

Room::operator Coordinate() {
	return coordinate;
}