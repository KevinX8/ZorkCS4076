#include "GameInstance.h"

using namespace std::placeholders;

GameInstance::GameInstance(bool loadGame, int seed) {
    //constructs a game instance
    //creates a File interface object, not yet implemented fully
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
        this-> floor = new Floor(0, 200);
        //floor->floorHexUnitTest();
        File::deleteSaves();
    }
     std::function<void(shared_ptr<Item>)> diRf = std::bind(&GameInstance::interactDropPlayerInv,this,std::placeholders::_1);
    playerInv = new InventoryWidget(player,diRf);
    setGUI();
}

void GameInstance::setGUI(){
    //sets the user interface for the game instance
    std::function<void(shared_ptr<Door>)> dRf = std::bind(&GameInstance::changeRoom,this,std::placeholders::_1);
    std::function<void(shared_ptr<NPC>)> nRf = std::bind(&GameInstance::interactNPC,this,std::placeholders::_1);
    std::function<void()> rRf = std::bind(&GameInstance::resetButtons,this);
    std::function<void(bool)> cfRf = std::bind(&GameInstance::changeFloor,this,std::placeholders::_1);
    std::function<void(int)> riRf = std::bind(&GameInstance::interactRoomItem,this,std::placeholders::_1);
    this-> playerRoomIndex = -1;
    for(Room r: floor->rooms){
        this->playerRoomIndex++;
        if(r.getDoors().size() > 1 && !r.getKiosk()){
            break;
        }
    }
    shared_ptr<MapWidget> map = shared_ptr<MapWidget>(new MapWidget(floorNumber, playerRoomIndex, *floor, floor->rooms.at(playerRoomIndex), dRf, nRf, rRf, cfRf));
    shared_ptr<RoomItemWidget> roomInv = shared_ptr<RoomItemWidget>(new RoomItemWidget(floor->rooms.at(playerRoomIndex).getItems(),riRf));
    shared_ptr<TextBoxWidget> textBox = shared_ptr<TextBoxWidget>(new TextBoxWidget("Try clicking on buttons on the map!",{"","","",""}));
    this->gui = unique_ptr<MainWindow>(new MainWindow(roomInv,map,textBox,playerInv));
    gui->show();
    resetButtons();
}

void GameInstance::resetButtons(){
    //resets option buttons so that the options and prompt are blank
    vector<QString> options = {"","","",""};
    gui->text->updateInteractions(options);
    gui->text->enableButtons(0);
    gui->text->updateTextBox("");
}

void GameInstance::changeFloor(bool up){
    //goes to the next floor
    if(givingItem){
        givingItem = false;
        resetDropFunc();
    }
    gameState.writeFloor(floor->floorToken(), floorNumber);
    if(floor->rooms.at(gui->map->currentRoomIndex).getKiosk()){
        int upgrade = rand() % 3;
        switch(upgrade){
            case(0): {player.strength++;break;};
            case(1): {player.luck++;break;};
            case(2): {player.charisma++;break;};
        }
        floor->rooms.at(gui->map->currentRoomIndex).removeKiosk();
    }
    delete(floor);
    if(up){
        floorNumber++;
        floor = new Floor(floorNumber, seed);
        setGUI();
    }else{
        floorNumber--;
        string token = gameState.readFloor(floorNumber);
        floor = new Floor(seed, floorNumber, token);
    }
}

void GameInstance::changeRoom(shared_ptr<Door> d){
    //move to the room which door d goes to
    /*
    0: door was unlocked and room got changed,
    1: door was locked and player has key,
    2: door was locked and player does not have key
    */
   if(givingItem){
        givingItem = false;
        resetDropFunc();
    }
    resetButtons();
    if(d->locked){
        bool keyFound = false;
        for(shared_ptr<Item> &item : player.inventory){
            if(item->hashCode == 0){
                gui->text->updateTextBox("The Door is locked. Use key?");
                vector<QString> options;
                options.push_back("Yes");
                options.push_back("No");
                options.push_back("");
                options.push_back("");
                gui->text->updateInteractions(options);
                gui->text->enableButtons(2);
                gui->text->funcBox1 = std::bind(&GameInstance::unlockDoor,this , d);
                gui->text->funcBox2 = [&](){resetButtons();};
                keyFound = true;
                break;
            }
        }
        if(!keyFound){
            gui->text->updateTextBox("The Door is locked. You have no key.");
        }
    }else{
        if(player.inventorySpace >= player.inventory.size()){
            resetButtons();
            gui->map->changeRoom(d->roomIndex);
            Room& r = gui->map->f.rooms.at(d->roomIndex);
            gui->room->updateItems(floor->rooms.at(gui->map->currentRoomIndex).getItems());
            if(r.getKiosk()){
                gui->text->updateTextBox("You reached the end of the Floor! Choose an ability to upgrade:");
                vector<QString> options;
                options.push_back("⚔️");
                options.push_back("🗣️");
                options.push_back("☘️");
                options.push_back("");
                gui->text->updateInteractions(options);
                gui->text->enableButtons(3);
                gui->text->funcBox1 = [&](){resetButtons(); player.strength++; gui->inv->updateStats(); r.removeKiosk();};
                gui->text->funcBox2 = [&](){resetButtons(); player.charisma++; gui->inv->updateStats(); r.removeKiosk();};
                gui->text->funcBox3 = [&](){resetButtons(); player.luck++; gui->inv->updateStats(); r.removeKiosk();};
            }
        }else{
            resetButtons();
            gui->text->updateTextBox("You can't move because you're \ncarrying too many items!");
        }
    }
}

void GameInstance::unlockDoor(shared_ptr<Door> d) {
    //unlocks door d
    resetButtons();
    int index = player.takeItem(0);
    d->locked = false;
    gui->map->resetButtons();
    gui->inv->updateInventory(index);
    gui->inv->updateStats();

}

void GameInstance::interactNPC(shared_ptr<NPC> npc) 
{
    //function called when player clicks on NPC button on the map. interacts with the npc
    if(givingItem){
        givingItem = false;
        resetDropFunc();
    }
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        QString info = QString::fromStdString("Type: Human\nName: " + h->getName() + "\n⚔️: " + to_string(h->getStrength()) + ", 🗣️: " + to_string(h->getCharisma()));
        gui->text->updateTextBox(info);
        setNPCButtons(npc);
    }
}

void GameInstance::setNPCButtons(shared_ptr<NPC> npc){
    //sets the npc interaction buttons.
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        vector<QString> options;
        options.push_back("Fight");
        options.push_back("Ask For Info");
        options.push_back("Give Item");
        options.push_back("Chat");
        gui->text->updateInteractions(options);
        std::function<void ()> fightFunc = std::bind(&GameInstance::fightNPC,this , npc);
        gui->text->enableButtons(4);
        gui->text->funcBox1 = std::bind(&GameInstance::fightNPC,this , npc);
        gui->text->funcBox2 = std::bind(&GameInstance::askInfoNPC, this, h);
        gui->text->funcBox3 = std::bind(&GameInstance::giveNPCItem, this, npc);
        gui->text->funcBox4 = std::bind(&GameInstance::startConvo, this, npc);
    }
}

void GameInstance::fightNPC(shared_ptr<NPC> npc){
    //called when the player trys to fight an NPC.
    bool wasEmpty = (player.inventory.size() == 0);
    int result = npc->fight(player);
    if(result == -1){
        QString info = QString::fromStdString("You won the fight! Should you spare " + npc->getName() + "?");
        gui->text->updateTextBox(info);
        gui->text->enableButtons(2);
        gui->text->funcBox1 = std::bind(&GameInstance::spareOrKill,this , npc, true);
        gui->text->funcBox2 = std::bind(&GameInstance::spareOrKill, this, npc, false);
        vector<QString> options;
        options.push_back("Spare");
        options.push_back("Kill");
        options.push_back("");
        options.push_back("");
        gui->text->updateInteractions(options);
    }else{
        QString info;
        if(wasEmpty){
            info = QString::fromStdString("You lost the fight! ... You're so poor that " + npc->getName() + " had nothing to steal!");
        }else{
            info = QString::fromStdString("You lost the fight! ... " + npc->getName() + " stole one of your items!");
            gui->inv->updateStats();
            gui->inv->updateInventory(result);

        }
        gui->text->updateTextBox(info);
        setNPCButtons(npc);
    }
}

void GameInstance::spareOrKill(shared_ptr<NPC> npc, bool spare){
    //called when the player either chooses to spare or kill an NPC after beating them in a fight.
    QString info = QString::fromStdString(npc->spareOrKill(spare, player, gui->inv));
    gui->text->updateTextBox(info);
    if(spare){
        setNPCButtons(npc);
    }else{
        resetButtons();
        gui->map->removeNPC(npc);
    }
}

void GameInstance::askInfoNPC(shared_ptr<Human> h){
    //called when the player asks an NPC for some useful info
    int code = h->askInfo(player);
    QString info;
    if(code == -2){
        info = QString::fromStdString( h->usefulInfo);
    }else if(code == -1){
        string text = "You failed to persuade " + h->getName() + ".";
        info = QString::fromStdString(text);
    }else{
        string text = "You failed to persuade " + h->getName() + ". " + h->getName() + " took one of your items!";
        info = QString::fromStdString(text);
        gui->inv->updateStats();
        gui->inv->updateInventory(code);
    }
    gui->text->updateTextBox(info);
}

void GameInstance::giveNPCItem(shared_ptr<NPC> npc){
    //changes the inventory widget so that the player can give an NPC an item
    gui->inv->giving = true;
    givingItem = true;
    gui->text->updateTextBox("Select an item to give from your inventory: ->");
    gui->text->enableButtons(0);
    gui->inv->dropFunc = std::bind(&GameInstance::continueGive,this,std::placeholders::_1);
    givingNPC = npc;
    gui->text->funcBox1 = std::bind(&GameInstance::interactNPC,this, npc);
    vector<QString> options;
    options.push_back("Cancel");
    options.push_back("");
    options.push_back("");
    options.push_back("");
    gui->text->updateInteractions(options);
    gui->text->enableButtons(1);
}

void GameInstance::resetDropFunc(){
    //stops the player from being able to give an item to npc
    gui->inv->dropFunc = std::bind(&GameInstance::interactDropPlayerInv,this,std::placeholders::_1);
    gui->inv->giving = false;
}

void GameInstance::continueGive(shared_ptr<Item> item){
    //called after the player selects an item to give to an NPC
    int count = 0;
    for(shared_ptr<Item> i : player.inventory){
        if(item->hashCode == i->hashCode){
            break;
        }else{
            count++;
        }
    }
    int result = givingNPC->giveItem(item->hashCode, player, gui->inv);
    QString info;
    if(result == -1){
        info = "I hate these. you can keep it.";
    }else{
        if(result == 0){
            info = "I love these. Here's a key I found.";
        }else{
            info = "I love these. Here's a token of appreciation.";
        }
        gui->inv->updateInventory(player.inventory.size()-1);
        gui->inv->updateStats();
    }
    resetDropFunc();
    givingItem = false;
    interactNPC(givingNPC);
    gui->text->updateTextBox(info);
}

void GameInstance::startConvo(shared_ptr<NPC> npc){
    //starts a conversation with an npc
    if(npc->getCode() < NUM_HUMANS){
        shared_ptr<Human> h = std::dynamic_pointer_cast<Human>(npc);
        chatNPC(npc, h->baseOption);
    }
}

void GameInstance::chatNPC(shared_ptr<NPC> npc, DialogueOption<string> d){
    //continues a conversation with an npc, given a dialogue options
    gui->text->updateTextBox(QString::fromStdString(d.reply));
    int count = 0;
    vector<DialogueOption<string>> nextOptions = d.getNextOption();
    auto it = nextOptions.begin();
    vector<QString> options;

    while(it != nextOptions.end() && count < 4){
        DialogueOption<string> r = (*it);
        string option = r.option, reply = r.reply;
        vector<DialogueOption<string>> nextOptions = r.nextOptions;
        shared_ptr<DialogueOption<string>> e = shared_ptr<DialogueOption<string>>(new DialogueOption<string>(option, reply, nextOptions));
        options.push_back(QString::fromStdString(r.option));
        switch (count) {
            case(0):{
                gui->text->funcBox1 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
            case(1):{
                gui->text->funcBox2 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
            case(2):{
                gui->text->funcBox3 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
            case(3): {
                gui->text->funcBox4 = [e, npc, this](){GameInstance::chatNPC(npc, *e);};
                break;
            }
        }
        count++;
        it++;
    }
    if(count == 0){
        setNPCButtons(npc);
    }else{
        gui->text->enableButtons(count);
        gui->text->updateInteractions(options);
    }
}

void GameInstance::interactRoomItem(int index) //user clicked pick it up
{
    player.addItem(floor->rooms.at(gui->map->currentRoomIndex).getItems().at(index));
    floor->rooms.at(gui->map->currentRoomIndex).removeItemFromRoom(index);
    gui->room->updateItems(floor->rooms.at(gui->map->currentRoomIndex).getItems());
    gui->inv->updateStats();
    gui->inv->updateInventory(player.inventory.size()-1);
}

void GameInstance::interactDropPlayerInv(shared_ptr<Item> item) //find item in player inventory and remove it, then add to room inventory
{
    floor->rooms.at(gui->map->currentRoomIndex).addItem(item->hashCode);
    int itemIndex = player.takeItem(item->hashCode);
    gui->room->updateItems(floor->rooms.at(gui->map->currentRoomIndex).getItems());
    gui->inv->updateInventory(itemIndex);
    gui->inv->updateStats();
}

QTGui::GameInstance::~GameInstance() 
{
    delete(floor);
    delete(playerInv);
}
