#include "Room.h"

Room::Room(unordered_set<int> cells){
    this->key = 9999999;
	for(int cellKey : cells){
        this->key = min(cellKey, key);
	}
	this->cells = cells;
    this->hasDownLadder = false;
    this->hasUpLadder = false;
}

void Room::addItem(int i) {
    //cout <<endl;
    //cout << "Just added" + inItem->getLongDescription();
    itemsInRoom.push_back(i);
}

void Room::removeItemFromRoom(int location) 
{
    itemsInRoom.erase(itemsInRoom.begin() + location);
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

vector<int> Room::getItems() 
{
    return itemsInRoom;
}

vector<Door> &Room::getDoors(){{}
    return doorsInRoom;
}

void Room::addDoor(Door& door){
    doorsInRoom.push_back(door);
}

Room::operator int() {
	return key;
}

bool Room::operator<(Room r2){
	return r2.getKey() >= this->getKey();
}

bool Room::operator==(Room * const r2){
    return r2->getKey() == this->getKey();
}

shared_ptr<NPC> Room::addNPC(int NPCKey, int floorNumber, bool emptyNPC){
    if(NPCKey < NUM_HUMANS){
        shared_ptr<NPC> npc(new Human(NPCKey, floorNumber, emptyNPC));
        npcsInRoom.push_back(npc);
        return npc;
    }
}

vector<shared_ptr<NPC>> &Room::getNPCs(){
    return npcsInRoom;
}

void Room::giveLadder(bool up){
    if(up){
        hasUpLadder = true;
    }else{
        hasDownLadder = true;
    }
}

bool Room::getKiosk() {
    return hasKiosk;
}

bool Room::containsUpLadder(){
    return hasUpLadder;
}

bool Room::containsDownLadder(){
    return hasDownLadder;
}


int* Room::upgradeStats(short statToUpgrade) {
//strength charisma luck invSpace
    int params[4] = {0,0,0,0};
    params[statToUpgrade] = 1;
    hasKiosk = false;
    return params;
}
