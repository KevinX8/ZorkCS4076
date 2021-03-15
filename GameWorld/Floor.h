#ifndef FLOOR_H_
#define FLOOR_H_
#include <vector>
#include <iostream>
#include <ctime>
#include <memory>
#include "Room.h"
#include "../Interaction/item/item.h"

using namespace std;
#define MAX_DIMENSION 15;
#define MIN_DIMENSION 5;

class Floor {
    private:
        bool disconnectedCell(int x,int y);
        bool cellOutOfBounds(int x, int y);
        float rangeRand();
        void generateRooms(vector<int> cells, int maxRoomSize);
        void generateNPCs(int floorNumber);
        void generateItems();
        void generateDoors();
        void generateLadders(bool firstFloor);
        Coordinate getRoomCoord(Coordinate foo);
        Coordinate getNextCell(Coordinate coord);
        void connectCells(Coordinate c1, Coordinate c2);
        void connectRooms(Room &r1, Room &r2, Coordinate c1, Coordinate c2);
        int height;
        vector<vector<Wall>> connections;
        Coordinate upLadder;
        Coordinate downLadder;
        Room &getRoom(int cellKey);
        void generateLockedDoors();
    public:
        Floor(int number,int seed = time(nullptr), bool previouslyGenerated = false);
        Floor(int seed, int number, string floorToken);
        vector<Room> rooms;
        vector<Door> doors;
        int getWidth();
        int getHeight();
        vector<vector<Wall>> getConnections();
};
#endif
