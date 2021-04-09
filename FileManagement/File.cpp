#include "File.h"

File::File(QString filePath,int gameSeed) {
    string saveObject;
    this->filePath = filePath.toStdString(); //folder with save stuff, ends with \ or /
    //open file just for output first so it gets created if it does not exist
    fstream saveFile;
    fstream floorFile;
    try {
    saveFile.open(filePath.toStdString() + "game.dat", std::ios::out | std::ios::app);
    saveFile.close();
    saveFile.open(filePath.toStdString() + "game.dat", std::ios::in | std::ios::out | std::ios::app);
    } catch (std::ifstream::failure &e) {
        qDebug() << "Exception opening file\n";
    }
    int line = 0;
    if (gameSeed != 0) {
    while (getline (saveFile, saveObject)) {
        if (line == 0) {
            gameSeed = stoi(saveObject);
        } else if (line == 1) {
            playerFloor = stoi(saveObject);
        } else if (line == 2) {
            playerToken = saveObject;
        } else if (line == -1) {
            break;
        }
        ++line;
        }
    } else {
        saveFile << gameSeed << "\n";
    }
    saveFile.close();
}

string File::readFloor(int floorNumber) {
    fstream saveFile;
    string saveObject;
    try {
        saveFile.open(filePath + to_string(floorNumber) + ".dat" , std::ios::in | std::ios::out | std::ios::app);
    } catch (std::ifstream::failure &e) {
        throw FloorFileException();
    }
    while (getline (saveFile, saveObject)) {
        return saveObject;
    }
    return "";
}


void File::writeFloor(string floorToken,int floorNumber) {
    QFile fileToRemove(QString::fromStdString(filePath + "\\floor\\" + to_string(floorNumber) + ".dat"));
    fileToRemove.remove();
    ofstream saveFile (filePath + "\\floor\\" + to_string(floorNumber) + ".dat" );
    if (saveFile.is_open()) {
        saveFile << floorToken;
    }
    saveFile.close();
}

void File::close(string playerToken, int floor) {
    QFile fileToRemove(QString::fromStdString(filePath + "game.dat"));
    fileToRemove.remove();
    ofstream saveFile (filePath + "game.dat" );
    if (saveFile.is_open()) {
        saveFile << gameSeed;
        saveFile << floor;
        saveFile << playerToken;
    }
    saveFile.close();
}

string File::getPlayerToken() {
    return playerToken;
}
int File::getGameSeed() {
    return gameSeed;
}
int File::getPlayerFloor() {
    return playerFloor;
}

void File::deleteSaves(QString filePath) {
    QFile fileToRemove(QString::fromStdString(filePath.toStdString() + "game.dat"));
    fileToRemove.remove();
    QDir dirToRemove(QString::fromStdString(filePath.toStdString() + "\\floor\\"));
    dirToRemove.removeRecursively();
}
