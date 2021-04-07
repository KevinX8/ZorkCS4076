#ifndef FLOOR_H_
#define FLOOR_H_
#include <vector>
#include <iostream>
#include <ctime>
#include <memory>
#include <cmath>
#include <algorithm>
#include <exception>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <iterator>
#include<QDebug>
#include "Room.h"
#include "../Interaction/item/item.h"

using namespace std;
#define MAX_DIMENSION 15;
#define MIN_DIMENSION 5;

struct LockedDoorException : public std::exception
{
	const char * what () const throw ()
    {
    	return "seed could not complete locked door generation!";
    }
};

struct GetRoomException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Failed to get room by cell key!";
    }
};

class Floor {
    private:
        bool disconnectedCell(int x,int y);
        bool cellOutOfBounds(int x, int y);
        float rangeRand();
        void lockDoor(Door& d);
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
        int seed;
        int number;
        vector<vector<Wall>> connections;
        Room &getRoom(int cellKey);
        Door& getOuterLockedDoor(int index);
        void generateLockedDoors();
        int getRoomIndex(Room& r);
    public:
        Floor(int number,int seed = time(nullptr), bool previouslyGenerated = false);
        Floor(int seed, int number, string floorToken);
        string floorToken();
        vector<Room> rooms;
        int getWidth();
        int getHeight();
        int downRoomIndex;
        vector<vector<Wall>>& getConnections();
        template<typename T>
        static int byteHexStringToInt(T first,T second);
        bool roomsUnitTest();
        void floorHexUnitTest();
};
#endif
