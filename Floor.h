#ifndef FLOOR_H_
#define FLOOR_H_
#include <vector>
#include <iostream>
#include <memory>
#include "Room.h"
#include "Item.h"
#include "Tools.h"

using namespace std;
#define MAX_DIMENSION 15;
#define MIN_DIMENSION 5;


struct Wall {
    unsigned int right : 2;
    /*
    0 means there's a wall to the right
    1 means there's no wall to the right
    2 means there's a wall with an unlocked door to the right.
    3 means there's a wall wiht a locked door to the right.
    */
    unsigned int down : 2;
};
struct Door {
    Coordinate doorLocation;
    bool vertical;
    Coordinate room1;
    Coordinate room2;
};

class Floor {
    private:
        bool disconnectedCell(int x,int y);
        bool cellOutOfBounds(int x, int y);
        float rangeRand();
        void generateRooms(vector<Coordinate> cells, int maxRoomSize);
        void generateItems();
        void generateItems(Room room);
        void generateDoors();
        Coordinate getRoomCoord(Coordinate foo);
        Room getRoom(Coordinate foo);
        Coordinate getNextCell(Coordinate coord);
        void connectCells(Coordinate c1, Coordinate c2);
        void connectRooms(Coordinate c1, Coordinate c2);
        vector<vector<Wall>> connections;
        int height;
        Coordinate upLadder;
        Coordinate downLadder;
    public:
        Floor(int number,int seed);
        vector<Room> rooms;
        vector<Door> doors;
        int getWidth();
        int getHeight();
        vector<vector<Wall>> getConnections();
};
#endif