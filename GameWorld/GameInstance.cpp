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

    resetButtons();
    
    this-> playerRoomIndex = 0;
}

void GameInstance::resetButtons(){
    vector<string> options;
    options.push_back("");
    options.push_back("");
    options.push_back("");
    options.push_back("");
    gui.text.updateInteractions(options);
    funcBox1 = []() {};
    funcBox2 = []() {};
    funcBox3 = []() {};
    funcBox4 = []() {};
    gui.text.updateTextBox("");
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
    resetButtons();
    if(d.locked){
        for(shared_ptr<Item> item : player.inventory){
            if(item->hashCode == 0){
                gui.text.updateTextBox("The Door is locked. Use key?");
                vector<string> options;
                options.push_back("Yes");
                options.push_back("No");
                options.push_back("");
                options.push_back("");
                gui.text.updateInteractions(options);
                funcBox1 = std::bind(&GameInstance::unlockDoor,this , d, item);
                funcBox2 = [&](){resetButtons();};
                funcBox3 = []() {};
                funcBox4 = []() {};
                return;
            }
        }
        gui.text.updateTextBox("The Door is locked.");
    }else{
        if(player.inventorySpace >= player.inventory.size()){
            resetButtons();
            Room& r = (*floor).rooms.at(d.roomIndex);
            gui.map.changeRoom(r);
            if(r.getKiosk()){
                gui.text.updateTextBox("You reached the end of the Floor! Choose an ability to upgrade:");
                vector<string> options;
                options.push_back("⚔️");
                options.push_back("🗣️");
                options.push_back("☘️");
                options.push_back("");
                gui.text.updateInteractions(options);
                funcBox1 = [&](){resetButtons(); player.strength++; gui.inv.updateStats();};
                funcBox2 = [&](){resetButtons(); player.charisma++; gui.inv.updateStats();};
                funcBox3 = [&](){resetButtons(); player.luck++; gui.inv.updateStats();};
                funcBox4 = []() {};
            }
        }else{
            gui.text.updateTextBox("You can't move because you're carrying too many items!");
            resetButtons();
        }
    }
}

void GameInstance::unlockDoor(Door& d, shared_ptr<Item> key) {
    resetButtons();
    this->interactDropPlayerInv(key);
    d.locked = false;
    gui.map.resetButtons();
}

void GameInstance::useKey(Door& d){
    for(auto it = player.inventory.begin(); it < player.inventory.end(); it++){
        if((*it)->hashCode == 0){
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

void GameInstance::interactNPC(shared_ptr<NPC> npc) 
{
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        gui.text.updateTextBox("Type: Human\nName: " + h->getName() + "\n⚔️: " + to_string(h->getStrength()) + ", 🗣️: " + to_string(h->getCharisma()));
        setNPCButtons(npc);
    }
}

void GameInstance::setNPCButtons(shared_ptr<NPC> npc){
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        vector<string> options;
        options.push_back("Fight");
        options.push_back("Ask For Info");
        options.push_back("Give Item");
        options.push_back("Chat");
        gui.text.updateInteractions(options);
        std::function<void ()> fightFunc = std::bind(&GameInstance::fightNPC,this , npc);
        funcBox1 = std::bind(&GameInstance::fightNPC,this , npc);
        funcBox2 = std::bind(&GameInstance::askInfoNPC, this, h);
        funcBox3 = std::bind(&GameInstance::giveNPCItem, this, npc);
        funcBox4 = std::bind(&GameInstance::chatNPC, this, npc);
    }
}

void GameInstance::fightNPC(shared_ptr<NPC> npc){
    if(npc->fight(player)){
        gui.text.updateTextBox("You won the fight! Should you spare " + npc->getName() + "?");
        funcBox1 = std::bind(&GameInstance::spareOrKill,this , npc, true);
        funcBox2 = std::bind(&GameInstance::spareOrKill, this, npc, false);
        funcBox3 = []() {};
        funcBox4 = []() {};
        vector<string> options;
        options.push_back("Spare");
        options.push_back("Kill");
        options.push_back("");
        options.push_back("");
        gui.text.updateInteractions(options);
    }else{
        gui.text.updateTextBox("You lost the fight! ... " + npc->getName() + " took one of your items!");
        setNPCButtons(npc);
    }
}

void GameInstance::spareOrKill(shared_ptr<NPC> npc, bool spare){
    gui.text.updateTextBox(npc->spareOrKill(spare, player));
    setNPCButtons(npc);
}

void GameInstance::askInfoNPC(shared_ptr<Human> h){
    gui.text.updateTextBox(h->askInfo(player));
}

void GameInstance::giveNPCItem(shared_ptr<NPC> npc){   

}

void GameInstance::chatNPC(shared_ptr<NPC> npc){

}

void GameInstance::interactRoomItem(int itemCode) //user clicked pick it up
{
    //not implemented
    gui.inv.updateStats();
}

void GameInstance::interactDropPlayerInv(shared_ptr<Item> item) //find item in player inventory and remove it, then add to room inventory
{
    //not implemented
    gui.inv.updateStats();
}