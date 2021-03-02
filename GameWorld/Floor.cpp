#include <cmath>
#include <ctime>
#include <algorithm>
#include <exception>
#include <sstream>
#include <unordered_set>
#include "Floor.h"

using namespace std;

Floor::Floor(int number, int seed = time(nullptr), bool previouslyGenerated = false){
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
    if(!previouslyGenerated){
        generateLockedDoors();
        //generate Items
    }
}

Floor::Floor(int number,int seed, string floorToken) {
    Floor(number,seed,true);
    int *offset = 0;
    for (int roomNo = 0; roomNo < byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset)); roomNo++) {
        /*rooms[roomNo]. set visited 2 3*/
        int size = byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset));//Number of items
        //offset += 2;
        if (size != 0) {
            rooms[roomNo].itemsInRoom.resize(size);
            for(int i = 0; i < size; i++){
                //rooms[roomNo].addItem(byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset)));             
                }
            //offset = offset + size * 2;
        }
        size = byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset));//Number of NPCs
        //offset += 2
        if (size != 0) {
            rooms[roomNo].npcsInRoom.resize(size);
            for(int npc = 0; npc < size; ++npc){
                int noItems = byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset));
                rooms[roomNo].getNPCs().resize(noItems);
                //offset += 2;
                for(int i = 0; i < noItems; ++i){
                    rooms[roomNo].getNPCs()[i].addItem(byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset)));
                    //offset += 2;      
                }
            }
        }
        size = byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset));//Number of Locked Doors
        if(size != 0){
            for(int i = 0; i < size; ++i){
                Coordinate c = Tools::getKeyCoordinate(byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset)));
                if(byteHexStringToInt(nextChar(floorToken, offset),nextChar(floorToken, offset))){
                    connections[c.y][c.x].right = 3;
                }else{
                    connections[c.y][c.x].down = 3;
                }
            }
        }
    }
}

inline char Floor::nextChar(string s, int *i) {
    char val = s.at(*i);
    ++*i;
    return val;
}

template<typename T>
inline int byteHexStringToInt(T first,T second) {
    stringstream ss;
    string number = "";
    ss << std::hex << (first << second);
    ss >> number;
    return stoi(number);
}

inline float Floor::rangeRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX+1);
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
        rooms.push_back(Room(currentRoom,"this is a room"));
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

void Floor::generateLockedDoors(){
    //Average number of 1 door rooms per floor is 8.6
    vector<Room> lockedRooms;
    unordered_set<int> lockedRoomKeys;

    for(Room &r : rooms){
        if(r.getDoors().size() == 1 && (rangeRand() * 8.6 < 1.0)){
            lockedRooms.push_back(r);
            lockedRoomKeys.insert(r.getKey());
        }
    }

    for(Room &r : lockedRooms){
        Door d = *r.getDoors().begin();
        d.locked = true;
        if(d.vertical){
            connections[d.doorLocation.y][d.doorLocation.x].right = 3;
        }else{
            connections[d.doorLocation.y][d.doorLocation.x].down = 3;
        }
        //MUST EITHER PLACE KEY IN ROOM< OR IN NPC INVENTORY IN A DIFFERENT ROOM HERE
    }

}

Room &Floor::getRoom(int cellKey){
    for(Room &r : rooms){
        if(r.cells.find(cellKey) != r.cells.end()){
            return r;
        }
    }
}

void Floor::connectRooms(Room r1, Room r2, Coordinate c1, Coordinate c2){
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
    d.roomKey1 = r1.getKey();
    d.roomKey2 = r2.getKey();
    doors.push_back(d);
    r1.addDoor(d);
    r2.addDoor(d);
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
/*
void Floor::generateItems() {
    
}

void Floor::generateItems(Room room){
    int amount_of_items = rand() % 3;
    for (int i = 0; i < amount_of_items; i++) {
        
    }
}
*/
int Floor::getWidth(){
    return Tools::width;
}

int Floor::getHeight(){
    return height;
}

vector<vector<Wall>> Floor::getConnections(){
    return connections;
}
