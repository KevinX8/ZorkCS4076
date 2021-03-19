#include "Player.h"
#include "Floor.h"
#include "../FileManagement/File.h"

class GameInstance {
    private:
        Player player;
        Floor* floor;
        int floorNumber;
        int seed;
        File gameState;
        Room* playerRoom;
    public:
        GameInstance(bool loadGame, int seed);
        void changeFloor(bool up);
        int changeRoom(Door& d);
        void useKey(Door& d);
        void useKiosk(int toUpgrade);
};