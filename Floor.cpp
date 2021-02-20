#include <cmath>
#include <ctime>
#include <algorithm>
#include <exception>
#include <unordered_set>
#include "Floor.h"
#include "Room.h"

using namespace std;

Floor::Floor(int number, int seed = time(nullptr)){
    Tools::width = 12;
    height = 10;
    srand(seed);
    vector<Coordinate> floorCells;
    //vector<vector<Wall>> *tempConnections* = new vector<;
    connections.resize(height);
    for(int r = 0; r < height; r++){
        connections[r].resize(Tools::width);
        for(int c = 0; c < Tools::width; c++){
            Coordinate cell;
            cell.x = c;
            cell.y = r;
            floorCells.push_back(cell);
            connections[r][c].down = 0;
            connections[r][c].right = 0;
        }
    }
    //connections = new vector(tempConnections.get_allocator());
    generateRooms(floorCells, 5);
    generateDoors();
}

inline float Floor::rangeRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX+1);
}

inline bool Floor::cellOutOfBounds(int x, int y) {
    return (x < 0 || x >= Tools::width || y >= height || y > 0);
}

Coordinate Floor::getNextCell(Coordinate coord){
    vector<Coordinate> allowedCells;
    vector<Coordinate>::iterator it;
    it = allowedCells.begin();
    int count = 0;
    for(int i = 0; i < 4; i++){
        int x = (i > 2)? coord.x + ((i+1)%2):coord.x + ((i+1)%2)*-1;
        int y = (i > 2)? coord.y + (i%2):coord.y + (i%2)*-1;
        if(disconnectedCell(x,y)){
            Coordinate foo;
            foo.x = x;
            foo.y = y;
            allowedCells.insert(it, foo);
            it++;
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

    return !((!cellOutOfBounds(x-1, y) && connections[x-1][y].right != 0)
        || (!cellOutOfBounds(x, y-1) && connections[x][y-1].down != 0)
        || (!cellOutOfBounds(x+1, y) && connections[x][y].right != 0)
        || (!cellOutOfBounds(x, y+1) && connections[x][y].down != 0));
}



void Floor::generateRooms(vector<Coordinate> unusedCells, int maxRoomSize = 4){
    vector<Coordinate> connectingCells;
    unordered_set<int> currentRoom;
    while(!unusedCells.empty()){
        int roomSize = (int)(rangeRand() * (maxRoomSize - 1)) + 1;
        vector<Coordinate>::iterator it;
        it = unusedCells.begin() + (int)(rangeRand() * unusedCells.size());
        connectingCells.push_back(*it);
        currentRoom.insert(Tools::getCoordinateKey(*it));
        unusedCells.erase(it);
        for(int i = 0; i < roomSize-1; ++i){
            it = connectingCells.begin() + (int)(rangeRand() * connectingCells.size());
            Coordinate nextCell = getNextCell(*it);
            if(nextCell.x == -1){
                connectingCells.erase(it);
                if(connectingCells.empty()){
                    //no more possible cells to connect to.
                    break;
                }
            }else{
                connectCells(*it, nextCell);
                currentRoom.insert(Tools::getCoordinateKey(*it));
            }
        }
        rooms.push_back(Room(currentRoom,"this is a room"));
        connectingCells.clear();   
        currentRoom.clear();             
    }
    //Maybe change to static cast rather than dynamic
    sort(rooms.begin(), rooms.end(), Tools::compareCoordinates);
}

void Floor::generateDoors(){
    vector <Room> connectedRooms;
    vector<Room>::iterator it;
    unordered_set<int>::iterator it2;
    it = rooms.begin() + (int)(rangeRand() * rooms.size());
    while (connectedRooms.size() < rooms.size()) {
        Room r = *it;
        for(it2 = r.getCells().begin();it2 != r.getCells().end(); it2++){
            //go through all cells in a room
            int cellKey = *it2;
            Coordinate cell = Tools::getKeyCoordinate(cellKey);
            int c = rangeRand() * 4;
            for(int i = c; i < c+4; i++){
                //check to see if any of the neighbouring cells can be joined with a door
                Coordinate offset;
                int j = i%4;
                offset.x += ((j) > 2)? cell.x + (j+1)%2 : cell.x + ((j+1)%2)*-1;
                offset.y += ((j) > 2)? cell.x + (j%2) : cell.x + (j%2)*-1;
                if(!(*it).cellInRoom(offset) && !cellOutOfBounds(cell.x , cell.y)){
                    //only connect if cell is not in current room or out of bounds
                    bool roomConnected = false;
                    for(Room room : connectedRooms){
                        if((getRoomCoord(offset)).x != -1){
                            roomConnected = true;
                            //don't connect to a room if it's already connected
                        }
                    }
                    if (!roomConnected) {
                        connectRooms(offset, cell);
                        connectedRooms.push_back(getRoom(offset));
                        return;
                    }
                }
            }
        }
        it = connectedRooms.begin() + (int)(rangeRand() * connectedRooms.size());
    }
}

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

Room Floor::getRoom(Coordinate foo){
    //WARNING ADD THROWS EXCEPTION LATER!!!!!!
    for(Room room : rooms){
        if(room.cellInRoom(foo)){
            return room;
        }
    }
    throw "Invalid Coordinate";
}

void Floor::connectRooms(Coordinate c1, Coordinate c2){
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
    doors.push_back(d);
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

void Floor::generateItems() {
    
}

void Floor::generateItems(Room room){
    int amount_of_items = rand() % 3;
    for (int i = 0; i < amount_of_items; i++) {
        
    }
}

int Floor::getWidth(){
    return Tools::width;
}

int Floor::getHeight(){
    return height;
}

vector<vector<Wall>> Floor::getConnections(){
    return connections;
}
