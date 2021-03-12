#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <fstream>
#include "..\GameWorld\Floor.h"
#define DEFAULT_PATH "./save.dat"

using namespace std;

class File {
    private:
        string playerToken,filePath;
        int gameSeed;
        int playerFloor;
    public:
        File();
        File(string filePath = DEFAULT_PATH, int gameSeed = 0);
        string getPlayerToken();
        string readFloor(int);
        void writeFloor(string floorToken,int floorNumber);
        void close(string playerToken);
        int getGameSeed();
        int getPlayerToken();
        int getPlayerFloor();
};
#endif