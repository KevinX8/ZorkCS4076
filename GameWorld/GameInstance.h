#ifndef GAMEINSTANCE_H_
#define GAMEINSTANCE_H_

#include "../interaction/Player.h"
#include "Floor.h"
#include "../FileManagement/File.h"
#include "..\QtGui\mainWindow.h"
#include <memory>
#include <functional>
#include <string>

class GameInstance {
    private:
        Player player;
        Floor* floor;
        int floorNumber;
        int seed;
        File gameState;
        int playerRoomIndex;
        MainWindow gui;
        void fightNPC(shared_ptr<NPC> npc);
        void askInfoNPC(shared_ptr<Human> h);
        void giveNPCItem(shared_ptr<NPC> npc);
        void chatNPC(shared_ptr<NPC> npc);
        void setNPCButtons(shared_ptr<NPC> npc);
        void resetButtons();
        void spareOrKill(shared_ptr<NPC> npc, bool spare);
    public:
        std::function<void()> funcBox1;
        std::function<void()> funcBox2;
        std::function<void()> funcBox3;
        std::function<void()> funcBox4;
        GameInstance(bool loadGame, int seed);
        void changeFloor(bool up);
        int changeRoom(Door& d);
        void useKey(Door& d);
        void useKiosk(int toUpgrade);
        void interactNPC(shared_ptr<NPC> npc);
        void interactRoomItem(int itemCode);
        void interactPlayerInv(Item item);
        void interactPlayerEquipment(int slot);
};

#endif