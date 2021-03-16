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

template<typename T>
int Tools::byteHexStringToInt(T first,T second) {
    stringstream ss;
    string number = "";
    ss << first;
    ss << second;
    ss << std::hex << ss.str();
    ss >> number;
    return stoi(number);
}

string Tools::intToByteHexString(int i){
    int first = i & 0b00001111;
    int second = (i & 0b11110000) >> 4;
    stringstream ss;
    string number = "";
    ss << std::hex << first;
    ss << std::hex << second;
    ss >> number;
    return number;
}

char Tools::nextChar(string s, int *i) {
    char val = s.at(*i);
    ++*i;
    return val;
}