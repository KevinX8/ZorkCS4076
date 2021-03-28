#include "../interaction/Player.h"
#include "Floor.h"
#include "../FileManagement/File.h"
#include "..\QtGui\mainWindow.h"
#include <memory>

class GameInstance {
    private:
        Player player;
        Floor* floor;
        int floorNumber;
        int seed;
        File gameState;
        int playerRoomIndex;
        MainWindow gui;
    public:
        GameInstance(bool loadGame, int seed);
        void changeFloor(bool up);
        int changeRoom(Door& d);
        void useKey(Door& d);
        void useKiosk(int toUpgrade);
};