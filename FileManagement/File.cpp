#include <QDebug>
#include "File.h"

using namespace std;

File::File(string filePath = DEFAULT_PATH, int floor = -1) {
    fstream saveFile;
    string saveObject;
    //open file just for output first so it gets created if it does not exist
    try {
    saveFile.open(filePath, std::ios::out | std::ios::app);
    saveFile.close();
    saveFile.open(filePath, std::ios::in | std::ios::out | std::ios::app);
    } catch (std::ifstream::failure e) {
        qDebug() << "Exception opening file\n";
    }
    int line = 0;
    while (getline (saveFile, saveObject)) {
        if (line == 0) {
            gameSeed = saveObject.at(0);
        } else if (line == 1) {
            playerToken = saveObject;
        } else if (line == floor+1) {
            floorToken = saveObject;
        } else if (line == -1) {
            break;
        }
        ++line;
    }
    saveFile.close();
}

string File::readFloor(int floorNumber) {

}

void File::writeFloor(string floorToken,int floorNumber) {
    
}
string File::getFloorToken() {
    return floorToken;
}
int File::getGameSeed() {
    return gameSeed;
}