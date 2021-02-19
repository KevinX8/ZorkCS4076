#include "Tools.h"

int Tools::width = 0;

bool Tools::compareCoordinates(Coordinate c1, Coordinate c2){
    return (c1.x == c2.x)? c1.y < c2.y : c1.x < c2.x;
}

int Tools::getCoordinateKey(Coordinate c){
    return (c.x % width) + (c.y * width);
}

Coordinate Tools::getKeyCoordinate(int key){
    Coordinate c;
    c.x = key % width;
    c.y = (key / width);
    return c;
}