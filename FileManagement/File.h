#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <fstream>
#include "..\GameWorld\Floor.h"
#define DEFAULT_PATH "./save.dat"

using namespace std;

class File {
    private:
        string floorToken,playerToken;
        int gameSeed;
    public:
        File();
        File(string filePath = DEFAULT_PATH, int floor = -1);
        string getFloorToken();
        string readFloor(int);
        void writeFloor(string floorToken,int floorNumber);

        int getGameSeed();
};
#endif