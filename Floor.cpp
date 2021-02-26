#include <cmath>
#include <ctime>
#include <algorithm>
#include <exception>
#include <unordered_set>
#include "Floor.h"

using namespace std;

Floor::Floor(int number, int seed = time(nullptr)){
    srand(seed);
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
    //connections = new vector(tempConnections.get_allocator());
    generateRooms(floorCells, 8);
    generateDoors();
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
    //Maybe change to static cast rather than dynamic
    sort(rooms.begin(), rooms.end(), Tools::compareKeys);
}

void Floor::generateDoors(){
    vector <Room> connectedRooms;
    vector<Room>::iterator it;
    unordered_set<int>::iterator it2;
    it = rooms.begin() + (int)(rangeRand() * rooms.size());
    while (rooms.size() > 0) {

        Room r = *it;
        bool roomConnected = false;

        for(it2 = r.cells.begin();it2 != r.getCells().end() && !roomConnected; ++it2){
            //go through all cells in a room until there is a cell that can
            int cellKey = *it2;
            Coordinate cell = Tools::getKeyCoordinate(cellKey);
            int c = rangeRand() * 4;

            for(int i = c; i < c+4 && !roomConnected; i++){
                //check to see if any of the neighbouring cells can be joined with a door
                Coordinate offset;
                int j = i%4;
                offset.x = cell.x + (j-1) * (j+1)%2;
                offset.y = cell.y + (j-2) * j%2;

                if(!(*it).cellInRoom(offset) && !cellOutOfBounds(offset.x , offset.y)){
                    //only connect if cell is not in current room or out of bounds
                    for(Room room : rooms){
                        if(room.cellInRoom(offset)){
                            connectRooms(room, *it, offset, cell);
                            rooms.erase(lower_bound(rooms.begin(), rooms.end(), room));
                            connectedRooms.push_back(room);
                            roomConnected = true;
                            break;
                        }
                    }
    
                }
            }

        }
        it = connectedRooms.begin() + (int)(rangeRand() * connectedRooms.size());
    }
    rooms = connectedRooms;
}
/*
Coordinate Floor::getRoomCoord(Coordinate foo){
    for(Room room : rooms){
        if(room.cellInRoom(foo)){
            return static_cast<Coordinate>(room);
        }
    }
    Coordinate c;
    c.x = -1;
    return c;
}
*/

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
        d.vertical = true;
    }else{
        if(c1.x < c2.x){
            connections[c1.y][c1.x].right = 2;
            d.doorLocation = c1;
        }else{
            connections[c2.y][c2.x].right = 2;
            d.doorLocation = c2;
        }
        d.vertical = false;
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
