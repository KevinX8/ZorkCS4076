#include "GameInstance.h"
#include <ctime>

GameInstance::GameInstance(bool loadGame, int seed = time(nullptr)){
    this-> gameState = File();

    if(loadGame){
        this-> seed = gameState.getGameSeed();
        this-> player = Player(gameState.getPlayerToken());
        this-> floorNumber = gameState.getPlayerFloor();
        string floorToken = gameState.readFloor(floorNumber);
        this-> floor = new Floor(seed, floorNumber, floorToken);
    }else{
        this-> seed = seed;
        this-> player = Player();
        this-> floorNumber = 0;
        this-> floor = new Floor(0, seed);
        File::deleteSaves();
    }
    
    this-> playerRoomIndex = 0;
}

void GameInstance::changeFloor(bool up){
    gameState.writeFloor(floor->floorToken(), floorNumber);
    delete(floor);
    if(up){
        floorNumber++;
        try{
            string token = gameState.readFloor(floorNumber);
            floor = new Floor(seed, floorNumber, token);
        }catch(FloorFileException f){
            floor = new Floor(floorNumber, seed);
        }
    }else{
        floorNumber--;
        string token = gameState.readFloor(floorNumber);
        floor = new Floor(seed, floorNumber, token);
    }
}

int GameInstance::changeRoom(Door& d){
    /*
    0: door was unlocked and room got changed,
    1: door was locked and player has key,
    2: door was locked and player does not have key
    */
    if(d.locked){
        for(Item item : player.inventory){
            if(item.hashCode == 0){
                return 1;
            }
        }
        return 2;
    }else{
        this->playerRoomIndex = d.roomIndex;
        return 0;
    }
}

void GameInstance::useKey(Door& d){
    vector<Item>::iterator it;
    for(it = player.inventory.begin(); it < player.inventory.end(); it++){
        if((*it).hashCode == 0){
            player.inventory.erase(it);
        }
    }
    d.locked = false;
    if(d.vertical){
        floor->getConnections()[d.doorLocation.y][d.doorLocation.x].right = 2;
    }else{
        floor->getConnections()[d.doorLocation.y][d.doorLocation.x].down = 2;
    }
}

void GameInstance::useKiosk(int toUpgrade){
    int* mods = floor->rooms[playerRoomIndex].upgradeStats(toUpgrade);
    player.changeParams(mods, true);
}