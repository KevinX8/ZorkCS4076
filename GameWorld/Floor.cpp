#include <cmath>
#include <ctime>
#include <algorithm>
#include <exception>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include "Floor.h"

#define NEXT_HEX byteHexStringToInt(Tools::nextChar(floorToken, offset),Tools::nextChar(floorToken, offset))

using namespace std;

inline float Floor::rangeRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX+1);
}

inline void Floor::lockDoor(Door& d){
    d.locked = true;
    if(d.vertical){
        connections[d.doorLocation.y][d.doorLocation.x].right = 3;
    }else{
        connections[d.doorLocation.y][d.doorLocation.x].down = 3;
    }
}

inline Door& Floor::getOuterLockedDoor(Room& r){
    Door& innerDoor = *r.getDoors().begin();
    Room& outerRoom = *(innerDoor.room);
    for(Door& d : outerRoom.getDoors()){
        if(*(d.room) == r){
            return d;
        }
    }
}

template<typename T>
int Floor::byteHexStringToInt(T first,T second) {
    stringstream ss;
    string number = "";
    ss << first;
    ss << second;
    ss << std::hex << ss.str();
    ss >> number;
    return stoi(number);
}

Floor::Floor(int number, int seed, bool previouslyGenerated){
    srand(seed+number);
    Tools::width = rangeRand() * 4 + 7;
    height = rangeRand() * 4 + 7;
    vector<int> floorCells;
    //vector<vector<Wall>> *tempConnections* = new vector<;
    connections.resize(height);
    for(int r = 0; r < height; r++){
        connections[r].resize(Tools::width);
        for(int c = 0; c < Tools::width; c++){
            Coordinate cell;
            cell.x = c;
            cell.y = r;
            floorCells.push_back(Tools::getCoordinateKey(cell));
            connections[r][c].down = 0;
            connections[r][c].right = 0;
        }
    }
    generateRooms(floorCells, 8);
    generateDoors();
    generateLadders(number == 0);
    if(!previouslyGenerated){
        generateItems();
        generateNPCs(number);
        generateLockedDoors();
    }
}

//USE A UNION HERE
Floor::Floor(int number,int seed, string floorToken) {
    Floor(number,seed,true);
    int *offset = 0;
    for (Room& room : rooms) {       
        bool visited = (NEXT_HEX != 0); // does nothing at the moment
        int numItemsInRoom = NEXT_HEX;
        for (int i =0; i < numItemsInRoom; ++i) { //add items to the room
            room.addItem(NEXT_HEX);
        }
        int numNPCsInRoom = NEXT_HEX;
        for (int i =0; i < numNPCsInRoom; ++i) {
            
            NPC* npc = room.addNPC(NEXT_HEX,number, true);
            int numItems = NEXT_HEX;
            for (int j=0; j < numItems; ++j) {
                npc->addItem(NEXT_HEX);
            }
        }
        if(NEXT_HEX){//door is locked in room
            Door& d = getOuterLockedDoor(room);
            lockDoor(d);
        }

    }
}

string Floor::floorToken() {
    stringstream sstream;
    string token = "";
    for (Room room : rooms) {
        //sstream << room.visited();
        sstream << Tools::intToByteHexString(room.itemsInRoom.size());
        for (int items : room.itemsInRoom) {
            sstream << Tools::intToByteHexString(items);
        }
        sstream << Tools::intToByteHexString(room.npcsInRoom.size());
        for (NPC *npc : room.getNPCs()) {
            sstream << Tools::intToByteHexString(npc->getCode());
            for (int items : npc->getInventory()) {
                sstream << Tools::intToByteHexString(items);
            }
        }
        sstream << Tools::intToByteHexString(room.getDoors().at(0).locked);
    }
    sstream >> token;
    return token;
}

inline bool Floor::cellOutOfBounds(int x, int y) {
    return (x < 0 || x >= Tools::width || y >= height || y < 0);
}

Coordinate Floor::getNextCell(Coordinate coord){
    vector<Coordinate> allowedCells;
    vector<Coordinate>::iterator it;
    int count = 0;
    for(int i = 0; i < 4; i++){
        int x = coord.x + (i-1) * (i+1)%2;
        int y = coord.y + (i-2) * i%2;
        if(!cellOutOfBounds(x, y) && disconnectedCell(x,y)){
            Coordinate foo;
            foo.x = x;
            foo.y = y;
            allowedCells.insert(allowedCells.end(), foo);
            count++;
        }
    }
    if(allowedCells.size()>0){
        it=allowedCells.begin() + (int)(rangeRand() * count);
        return *it;
    }else{
        Coordinate noValidCell;
        noValidCell.x = -1;
        return noValidCell;
    }
}

bool Floor::disconnectedCell(int x, int y){
    //method returns true if the cell at coordinates x,y is not connected to any other cell
    //Simplifying the logic here would result in fragmentation errors

    return !((!cellOutOfBounds(x-1, y) && connections[y][x-1].right != 0)
        || (!cellOutOfBounds(x, y-1) && connections[y-1][x].down != 0)
        || (!cellOutOfBounds(x+1, y) && connections[y][x].right != 0)
        || (!cellOutOfBounds(x, y+1) && connections[y][x].down != 0));
}

void Floor::generateRooms(vector<int> unusedCells, int maxRoomSize = 7){
    vector<int> connectingCells;
    unordered_set<int> currentRoom;
    while(!unusedCells.empty()){
        int roomSize = (int)(rangeRand() * (maxRoomSize - 4)) + 4;
        vector<int>::iterator it;
        it = unusedCells.begin() + (int)(rangeRand() * unusedCells.size());
        connectingCells.push_back(*it);
        currentRoom.insert(currentRoom.end(), *it);
        unusedCells.erase(it);
        for(int i = 0; i < roomSize-1; ++i){
            it = connectingCells.begin() + (int)(rangeRand() * connectingCells.size());
            Coordinate nextCell = getNextCell(Tools::getKeyCoordinate(*it));
            if(nextCell.x == -1){
                connectingCells.erase(it);
                if(connectingCells.empty()){
                    //no more possible cells to connect to.
                    break;
                }
            }else{

                connectCells(Tools::getKeyCoordinate(*it), nextCell);
                if (i>=2) { //find cells in room next to each other and connect them
                    Coordinate tempCell;
                    for (int k = 0; k < 4; k++) {
                        tempCell.x = nextCell.x + (k-1) * (k+1)%2; // -1 0 1 0
                        tempCell.y = nextCell.y + (k-2) * k%2;     // 0 -1 0 1
                        if (!cellOutOfBounds(tempCell.x, tempCell.y) && currentRoom.find(Tools::getCoordinateKey(tempCell)) != currentRoom.end()) {
                            connectCells(tempCell,nextCell);
                        }
                    }
                }
                connectingCells.push_back(Tools::getCoordinateKey(nextCell));
                currentRoom.insert(Tools::getCoordinateKey(nextCell));
                unusedCells.erase(lower_bound(unusedCells.begin(), unusedCells.end(), Tools::getCoordinateKey(nextCell)));
            }
        }
        rooms.push_back(Room(currentRoom));
        connectingCells.clear();   
        currentRoom.clear();             
    }
}

void Floor::generateDoors(){
    vector<Room>::iterator it;
    vector<int> connectedCells;//cells that are connected excluding cells which have no more cells to connect to
    vector<int> deadCells;//cells that are connected
    it = rooms.begin() + (int)(rangeRand() * rooms.size());
    Room r = *it;
    for(int cell : r.cells){
        connectedCells.insert(lower_bound(connectedCells.begin(),connectedCells.end(),cell),cell);
        deadCells.insert(lower_bound(deadCells.begin(),deadCells.end(),cell),cell);
    }
    int count = rooms.size()-1;
    while(count > 0){
        vector<int>::iterator it2;
        bool cellNotFound = true;
        Coordinate outerCell, innerCell;
        while(cellNotFound){
            it2 = connectedCells.begin() + (int)(rangeRand() * connectedCells.size());
            int c = rangeRand() * 4;
            innerCell = Tools::getKeyCoordinate(*it2);
            for(int i = c; i < c+4 && cellNotFound; i++){
                int j = i%4;
                outerCell.x = innerCell.x + (j-1) * (j+1)%2;
                outerCell.y = innerCell.y + (j-2) * j%2;
                if(!cellOutOfBounds(outerCell.x, outerCell.y) && !binary_search(deadCells.begin(),deadCells.end(),Tools::getCoordinateKey(outerCell))){
                    cellNotFound = false;
                }else{
                    if(i == c +3){
                        //all surrounding cells are either already connected, or else out of bounds.
                        connectedCells.erase(lower_bound(connectedCells.begin(),connectedCells.end(),*it2));
                    }
                }
            }
        }
        Room &innerRoom = getRoom(Tools::getCoordinateKey(innerCell));
        Room &outerRoom = getRoom(Tools::getCoordinateKey(outerCell));
        for(int cell : outerRoom.cells){
            deadCells.insert(lower_bound(deadCells.begin(),deadCells.end(),cell),cell);
            connectedCells.insert(lower_bound(connectedCells.begin(),connectedCells.end(),cell),cell);
        }
        connectRooms(innerRoom, outerRoom, innerCell, outerCell);
        count--;
    }     
}

void Floor::generateNPCs(int floorNumber){
    int numberOfNPCs = (rooms.size() / 4);
    numberOfNPCs *= 1 + ((rand()%11)-5)/50;
    for(int i = 0; i< numberOfNPCs; i++){
        vector<Room>::iterator it;
        it = rooms.begin() + (int)(rand() % rooms.size());
        int key = rand() % NUM_NPCS;
        if((*it).getNPCs().size() < (*it).getCells().size()){
            (*it).addNPC(key, floorNumber);
        }else{
            i--;
        }
    }
}

void Floor::generateLockedDoors(){
    //Average number of 1 door rooms per floor is 8.6
    vector<Room> lockedRooms;
    unordered_set<int> lockedRoomKeys;
    const float averageLockedDoor = 0.15;
    for(Room &r : rooms){
        if(r.getDoors().size() == 1 && (rangeRand() < averageLockedDoor)){
            lockedRooms.push_back(r);
            lockedRoomKeys.insert(r.getKey());
        }
    }
    for(Room &r : lockedRooms){
        Door &d = getOuterLockedDoor(r);
        lockDoor(d);
        Room keyRoom = *(rooms.begin() + (int)(rand() % rooms.size()));
        while(keyRoom.getDoors().size() < 2){
            keyRoom = *(rooms.begin() + (int)(rand() % rooms.size()));
        }
        if(keyRoom.getNPCs().size() > 0){
            int likedItem = 0;
            Room likedItemRoom = *(rooms.begin());
            NPC* npc = dynamic_cast<NPC*>(*(keyRoom.getNPCs().begin() + (int)(rand() % keyRoom.getNPCs().size())));
            npc->giveKey();
            likedItem = npc->getLikedItem();
            likedItemRoom = *(rooms.begin() + (int)(rand() % rooms.size()));            
            while((int)likedItemRoom == ((int)keyRoom) || likedItemRoom.getDoors().size() < 2){
                likedItemRoom = *(rooms.begin() + (int)(rand() % rooms.size()));
            }
            likedItemRoom.addItem(likedItem);
        }else{
            keyRoom.addItem(0);
        }
    }

}

void Floor::generateItems(){
    int numberOfItems = (rooms.size() / 2);
    numberOfItems *= 1 + ((rand()%11)-5)/50;
    for(int i = 0; i < numberOfItems; i++){
        Room& itemRoom = *(rooms.begin() + (int)(rand() % rooms.size()));
        int rarity = rand() % 7;
        rarity /= 2;
        rarity = max(rarity, 1);
        if(itemRoom.getDoors().size() == 1){
            rarity += 1;
        }
        vector<short> possibleItems = Item::itemRarity.at(rarity);
        itemRoom.addItem(possibleItems.at(rand() % possibleItems.size()));
    }
}

void Floor::generateLadders(bool firstFloor = false){
    Room& upRoom = *(rooms.begin() + (int)(rand() % rooms.size()));
    upRoom.giveLadder(true);
    vector<Room>::iterator it = (rooms.begin() + (int)(rand() % rooms.size()));
    Room& downRoom = *it;
    while(downRoom.getDoors().size() < 2 && (int)downRoom == (int)upRoom){
        it = (rooms.begin() + (int)(rand() % rooms.size()));
        downRoom = *it;
    }
    
    rooms.erase(it);
    rooms.insert(rooms.begin(), downRoom);
    if(!firstFloor){
        downRoom.giveLadder(false);
    }
}

Room &Floor::getRoom(int cellKey){
    for(Room &r : rooms){
        if(r.cells.find(cellKey) != r.cells.end()){
            return r;
        }
    }
}

void Floor::connectRooms(Room &r1, Room &r2, Coordinate c1, Coordinate c2){
    Door d;
    if(c1.x == c2.x){
        if(c1.y < c2.y){
            connections[c1.y][c1.x].down = 2;
            d.doorLocation = c1;
        }else{
            connections[c2.y][c2.x].down = 2;
            d.doorLocation = c2;
        }
        d.vertical = false;
    }else{
        if(c1.x < c2.x){
            connections[c1.y][c1.x].right = 2;
            d.doorLocation = c1;
        }else{
            connections[c2.y][c2.x].right = 2;
            d.doorLocation = c2;
        }
        d.vertical = true;
    }
    d.locked = false;
    d.room = &r2;
    Door d2 = d;
    d2.room = &r1;
    r1.addDoor(d);
    r2.addDoor(d2);
}

void Floor::connectCells(Coordinate c1, Coordinate c2){
    if(c1.x == c2.x){
        if(c1.y < c2.y){
            connections[c1.y][c1.x].down = 1;
        }else{
            connections[c2.y][c2.x].down = 1;
        }
    }else{
        if(c1.x < c2.x){
            connections[c1.y][c1.x].right = 1;
        }else{
            connections[c2.y][c2.x].right = 1;
        }
    }
}

int Floor::getWidth(){
    return Tools::width;
}

int Floor::getHeight(){
    return height;
}

vector<vector<Wall>>& Floor::getConnections(){
    return connections;
}
