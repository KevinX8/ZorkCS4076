#include "Tools.h"

int Tools::width = 0;

bool Tools::compareKeys(int k1, int k2){
    return k1 < k2;
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