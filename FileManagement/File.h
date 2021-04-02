#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <fstream>
#include "..\GameWorld\Floor.h"
#define DEFAULT_PATH "./"

using namespace std;

struct FloorFileException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Floor not found";
    }
};

class File {
    private:
        string playerToken, filePath;
        int gameSeed;
        int playerFloor;
    public:
        File(string filePath = DEFAULT_PATH, int gameSeed = 0);
        string getPlayerToken();
        string readFloor(int);
        void writeFloor(string floorToken,int floorNumber);
        void close(string playerToken, int floor);
        int getGameSeed();
        int getPlayerFloor();
        static void deleteSaves(string filePath = DEFAULT_PATH);
};
#endif