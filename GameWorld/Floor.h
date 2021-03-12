#ifndef FLOOR_H_
#define FLOOR_H_
#include <vector>
#include <iostream>
#include <ctime>
#include <memory>
#include "Room.h"

using namespace std;
#define MAX_DIMENSION 15;
#define MIN_DIMENSION 5;

template<typename T>
inline int byteHexStringToInt(T first,T second) {
    stringstream ss;
    string number = "";
    ss << first;
    ss << second;
    ss << std::hex << ss.str();
    ss >> number;
    return stoi(number);
}

inline char Floor::nextChar(string s, int *i) {
    char val = s.at(*i);
    ++*i;
    return val;
}

inline float Floor::rangeRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX+1);
}

inline bool Floor::cellOutOfBounds(int x, int y) {
    return (x < 0 || x >= Tools::width || y >= height || y < 0);
}
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
        char nextChar(string s, int *i);
        vector<Room> rooms;
        vector<Door> doors;
        int getWidth();
        int getHeight();
        vector<vector<Wall>> getConnections();
};
#endif
