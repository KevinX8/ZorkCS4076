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

string Tools::intToByteHexString(int i){
    //converts an into a hex string eg. int 50 -> string "32"
    int second = i & 0b00001111;
    int first = (i & 0b11110000) >> 4;
    stringstream ss;
    string number = "";
    ss << std::hex << first;
    ss << std::hex << second;
    ss >> number;
    return number;
}