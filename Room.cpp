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

string Room::longDescription() {
	return "room = " + description + ".\n" + displayItem() + exitString();
}

string Room::exitString() {
	string returnString = "\nexits =";
	for (map<string, Room*>::iterator i = exits.begin(); i != exits.end(); i++)
		// Loop through map
		returnString += "  " + i->first;	// access the "first" element of the pair (direction as a string)
	return returnString;
}

Room* Room::nextRoom(string direction) {
	map<string, Room*>::iterator next = exits.find(direction); //returns an iterator for the "pair"
	if (next == exits.end())
		return NULL; // if exits.end() was returned, there's no room in that direction.
	return next->second; // If there is a room, remove the "second" (Room*)
				// part of the "pair" (<string, Room*>) and return it.
}

void Room::addItem(Item *inItem) {
    //cout <<endl;
    //cout << "Just added" + inItem->getLongDescription();
    itemsInRoom.push_back(*inItem);
}

string Room::displayItem() {
    string tempString = "items in room = ";
    int sizeItems = (itemsInRoom.size());
    if (itemsInRoom.size() < 1) {
        tempString = "no items in room";
        }
    else if (itemsInRoom.size() > 0) {
       int x = (0);
        for (int n = sizeItems; n > 0; n--) {
            tempString = tempString + itemsInRoom[x].getShortDescription() + "  " ;
            x++;
            }
        }
    return tempString;
    }

int Room::numberOfItems() {
    return itemsInRoom.size();
}

bool Room::cellInRoom(Coordinate c){
	return cells.find(Tools::getCoordinateKey(c)) != cells.end();
}

unordered_set<int> Room::getCells(){
	return cells;
}