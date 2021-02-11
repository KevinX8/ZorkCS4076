#ifndef FLOOR_H_
#define FLOOR_H_
#include <vector>
#include <iostream>
#include <memory>

using namespace std;
#define MAX_DIMENSION 15;
#define MIN_DIMENSION 5;


struct Wall {
    unsigned int right : 2;
    unsigned int down : 2;
};
struct Cells {
    bool up;
    bool down;
    bool left;
    bool right;
};
struct Coordinate {
    int x;
    int y;
};

class Floor {
    private:
        bool disconnectedCell(int x,int y);
        bool cellOutOfBounds(int x, int y);
        Coordinate getNextCell(Coordinate coord);
        unique_ptr<unique_ptr<Wall[]>[]> connections;
        int width, height;
    public:
        Floor(int number);
};
#endif