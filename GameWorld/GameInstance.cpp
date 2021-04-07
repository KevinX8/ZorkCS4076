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
    vector<QString> options;
    options.push_back("");
    options.push_back("");
    options.push_back("");
    options.push_back("");
    gui.text->updateInteractions(options);
    gui.text->funcBox1 = []() {};
    gui.text->funcBox2 = []() {};
    gui.text->funcBox3 = []() {};
    gui.text->funcBox4 = []() {};
    gui.text->updateTextBox("");
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

void GameInstance::changeRoom(Door& d){
    /*
    0: door was unlocked and room got changed,
    1: door was locked and player has key,
    2: door was locked and player does not have key
    */
    resetButtons();
    if(d.locked){
        bool keyFound = false;
        for(shared_ptr<Item> item : player.inventory){
            if(item->hashCode == 0){
                gui.text->updateTextBox("The Door is locked. Use key?");
                vector<QString> options;
                options.push_back("Yes");
                options.push_back("No");
                options.push_back("");
                options.push_back("");
                gui.text->updateInteractions(options);
                gui.text->funcBox1 = std::bind(&GameInstance::unlockDoor,this , d, item);
                gui.text->funcBox2 = [&](){resetButtons();};
                gui.text->funcBox3 = []() {};
                gui.text->funcBox4 = []() {};
                keyFound = true;
            }
        }
        if(keyFound){
            gui.text->updateTextBox("The Door is locked.");
        }
    }else{
        if(player.inventorySpace >= player.inventory.size()){
            resetButtons();
            Room& r = (*floor).rooms.at(d.roomIndex);
            gui.map->changeRoom(r);
            if(r.getKiosk()){
                gui.text->updateTextBox("You reached the end of the Floor! Choose an ability to upgrade:");
                vector<QString> options;
                options.push_back("⚔️");
                options.push_back("🗣️");
                options.push_back("☘️");
                options.push_back("");
                gui.text->updateInteractions(options);
                gui.text->funcBox1 = [&](){resetButtons(); player.strength++; gui.inv->updateStats();};
                gui.text->funcBox2 = [&](){resetButtons(); player.charisma++; gui.inv->updateStats();};
                gui.text->funcBox3 = [&](){resetButtons(); player.luck++; gui.inv->updateStats();};
                gui.text->funcBox4 = []() {};
            }
        }else{
            gui.text->updateTextBox("You can't move because you're carrying too many items!");
            resetButtons();
        }
    }
}

void GameInstance::unlockDoor(Door& d, shared_ptr<Item> key) {
    resetButtons();
    this->interactDropPlayerInv(key);
    d.locked = false;
    gui.map->resetButtons();
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
        QString info = QString::fromStdString("Type: Human\nName: " + h->getName() + "\n⚔️: " + to_string(h->getStrength()) + ", 🗣️: " + to_string(h->getCharisma()));
        gui.text->updateTextBox(info);
        setNPCButtons(npc);
    }
}

void GameInstance::setNPCButtons(shared_ptr<NPC> npc){
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        vector<QString> options;
        options.push_back("Fight");
        options.push_back("Ask For Info");
        options.push_back("Give Item");
        options.push_back("Chat");
        gui.text->updateInteractions(options);
        std::function<void ()> fightFunc = std::bind(&GameInstance::fightNPC,this , npc);
        gui.text->funcBox1 = std::bind(&GameInstance::fightNPC,this , npc);
        gui.text->funcBox2 = std::bind(&GameInstance::askInfoNPC, this, h);
        gui.text->funcBox3 = std::bind(&GameInstance::giveNPCItem, this, npc);
        gui.text->funcBox4 = std::bind(&GameInstance::startConvo, this, npc);
    }
}

void GameInstance::fightNPC(shared_ptr<NPC> npc){
    if(npc->fight(player)){
        QString info = QString::fromStdString("You won the fight! Should you spare " + npc->getName() + "?");
        gui.text->updateTextBox(info);
        gui.text->funcBox1 = std::bind(&GameInstance::spareOrKill,this , npc, true);
        gui.text->funcBox2 = std::bind(&GameInstance::spareOrKill, this, npc, false);
        gui.text->funcBox3 = []() {};
        gui.text->funcBox4 = []() {};
        vector<QString> options;
        options.push_back("Spare");
        options.push_back("Kill");
        options.push_back("");
        options.push_back("");
        gui.text->updateInteractions(options);
    }else{
        QString info = QString::fromStdString("You lost the fight! ... " + npc->getName() + " took one of your items!");
        gui.text->updateTextBox(info);
        setNPCButtons(npc);
    }
}

void GameInstance::spareOrKill(shared_ptr<NPC> npc, bool spare){
    QString info = QString::fromStdString(npc->spareOrKill(spare, player));
    gui.text->updateTextBox(info);
    if(spare){
        setNPCButtons(npc);
    }else{
        resetButtons();
        gui.map->removeNPC(npc);
    }
}

void GameInstance::askInfoNPC(shared_ptr<Human> h){
    QString info = QString::fromStdString(h->askInfo(player));
    gui.text->updateTextBox(info);
}

void GameInstance::giveNPCItem(shared_ptr<NPC> npc){   

}

void GameInstance::startConvo(shared_ptr<NPC> npc){
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        chatNPC(npc, h->baseOption);
    }
}

void GameInstance::chatNPC(shared_ptr<NPC> npc, DialogueOption<string> d){

    gui.text->updateTextBox(QString::fromStdString(d.option));
    int count = 0;
    vector<DialogueOption<string>> nextOptions = d.getNextOption();
    auto it = nextOptions.begin();
    vector<QString> options;

    while(it != nextOptions.end() && count < 4){
        DialogueOption<string> r = (*it);
        string option = r.option, reply = r.reply;
        vector<DialogueOption<string>> nextOptions = r.nextOptions;
        shared_ptr<DialogueOption<string>> e = shared_ptr<DialogueOption<string>>(new DialogueOption<string>(option, reply, nextOptions));
        options.push_back(QString::fromStdString(r.reply));
        switch (count) {
            case(0):{
                gui.text->funcBox1 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
            case(1):{
                gui.text->funcBox2 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
            case(2):{
                gui.text->funcBox3 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
            case(3): {
                gui.text->funcBox4 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
        }
        options.push_back(QString::fromStdString(r.reply));
        count++;
        it++;
    }
    if(count == 0){
        setNPCButtons(npc);
    }else{
        while(count < 4){
            options.push_back("");
            switch (count) {
                case(0):{
                    gui.text->funcBox1 = [](){};
                    break;
                }
                case(1):{
                    gui.text->funcBox2 = [](){};
                    break;
                }
                case(2):{
                    gui.text->funcBox3 = [](){};
                    break;
                }
                case(3): {
                    gui.text->funcBox4 = [](){};
                    break;
                }
            }
            count++;
        }
    }
    gui.text->updateInteractions(options);
}

void GameInstance::interactRoomItem(int itemCode) //user clicked pick it up
{
    //not implemented
    gui.inv->updateStats();
}

void GameInstance::interactDropPlayerInv(shared_ptr<Item> item) //find item in player inventory and remove it, then add to room inventory
{
    //not implemented
    gui.inv->updateStats();
}
