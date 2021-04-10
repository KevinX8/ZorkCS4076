#ifndef GAMEINSTANCE_H_
#define GAMEINSTANCE_H_

#include "Floor.h"
#include "../FileManagement/File.h"
#include <memory>
#include <ctime>
#include <functional>
#include <string>
#include "../QTGUI/MainWindow.h"

namespace QTGui {
    class GameInstance;
}

using namespace QTGui;

class QTGui::GameInstance {
    private:
        Player player;
        Floor* floor;
        int floorNumber;
        int seed;
        File gameState;
        int playerRoomIndex;
        unique_ptr<MainWindow> gui;
        shared_ptr<NPC> givingNPC;
        void fightNPC(shared_ptr<NPC> npc);
        void askInfoNPC(shared_ptr<Human> h);
        void giveNPCItem(shared_ptr<NPC> npc);
        void setNPCButtons(shared_ptr<NPC> npc);
        void spareOrKill(shared_ptr<NPC> npc, bool spare);
        void unlockDoor(shared_ptr<Door> d);
        void startConvo(shared_ptr<NPC> npc);
        void continueGive(shared_ptr<Item> item);
        void chatNPC(shared_ptr<NPC> npc, DialogueOption<string> d);
        void setGUI();
        void resetDropFunc();
        bool givingItem = false;
    public:
        void resetButtons();
        GameInstance(bool loadGame = false, int seed = time(nullptr));
        ~GameInstance();
        void changeFloor(bool up);
        void changeRoom(shared_ptr<Door> d);
        void useKey(shared_ptr<Door> d);
        void interactNPC(shared_ptr<NPC> npc);
        void interactRoomItem(int itemCode);
        void interactDropPlayerInv(shared_ptr<Item> item);
};

#endif
