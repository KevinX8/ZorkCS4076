#ifndef GAMEINSTANCE_H_
#define GAMEINSTANCE_H_

#include "Floor.h"
#include "../FileManagement/File.h"
#include <memory>
#include <functional>
#include <string>
#include "../QTGUI/mainWindow.h"

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
        void setNPCButtons(shared_ptr<NPC> npc);
        void spareOrKill(shared_ptr<NPC> npc, bool spare);
        void unlockDoor(Door& d, shared_ptr<Item> key);
        void startConvo(shared_ptr<NPC> npc);
        void chatNPC(shared_ptr<NPC> npc, DialogueOption<string> d);
    public:
        void resetButtons();
        GameInstance(bool loadGame, int seed);
        void changeFloor(bool up);
        void changeRoom(Door& d);
        void useKey(Door& d);
        void useKiosk(int toUpgrade);
        void interactNPC(shared_ptr<NPC> npc);
        void interactRoomItem(int itemCode);
        void interactDropPlayerInv(shared_ptr<Item> item);
};

#endif