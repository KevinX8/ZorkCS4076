#ifndef FLOOR_H_
#define FLOOR_H_
#include <vector>
#include <iostream>
#include <memory>
#include "Room.h"

using namespace std;
#define MAX_DIMENSION 15;
#define MIN_DIMENSION 5;

class Floor {
    private:
        bool disconnectedCell(int x,int y);
        bool cellOutOfBounds(int x, int y);
        float rangeRand();
        void generateRooms(vector<int> cells, int maxRoomSize);
        //void generateItems();
        //void generateItems(Room room);
        void generateDoors();
        Coordinate getRoomCoord(Coordinate foo);
        Coordinate getNextCell(Coordinate coord);
        void connectCells(Coordinate c1, Coordinate c2);
        void connectRooms(Room r1, Room r2, Coordinate c1, Coordinate c2);
        int height;
        vector<vector<Wall>> connections;
        Coordinate upLadder;
        Coordinate downLadder;
        Room getRoom(int cellKey);
    public:
        explicit Floor(int number,int seed);
        vector<Room> rooms;
        vector<Door> doors;
        int getWidth();
        int getHeight();
        vector<vector<Wall>> getConnections();
};
#endif
