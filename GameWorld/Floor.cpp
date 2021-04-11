#include "Floor.h"

#define DEBUG true

using namespace std;

inline void Floor::lockDoor(shared_ptr<Door> d){
    //locks a door
    d->locked = true;
    if(d->vertical){
        connections[d->doorLocation.y][d->doorLocation.x].right = 3;
    }else{
        connections[d->doorLocation.y][d->doorLocation.x].down = 3;
    }
}

inline shared_ptr<Door> Floor::getOuterLockedDoor(int innerRoomIndex) {
    //gets the door that enters into a locked room
    auto innerDoor = rooms.at(innerRoomIndex).getDoors().begin();
    Room& outerRoom = rooms.at(innerDoor->get()->roomIndex);
    for(shared_ptr<Door> d : outerRoom.getDoors()){
        if(d->roomIndex == innerRoomIndex){
            return d;
        }
    }
    throw LockedDoorException();
}

template<typename T>
int Floor::byteHexStringToInt(T first,T second) {
    //converts hex to int. eg. 0A -> 10
    stringstream ss;
    string number = "";
    ss << first;
    ss << second;
    string temp;
    ss >> temp;
    ss.clear();
    ss << std::hex << temp;
    ss >> number;
    return stoi(number);
}

bool Floor::roomsUnitTest() {
    //tests that all rooms are connected with doors properly
    for (Room& room : rooms) {
        for (shared_ptr<Door> door : room.getDoors()) {
            bool found = false;
            for (shared_ptr<Door> innerDoor : rooms.at(door->roomIndex).getDoors()) {
                if (innerDoor->roomIndex == getRoomIndex(room)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}

void Floor::floorHexUnitTest() 
{
    //tests if floor generation from tokens works properly
    string firstToken = floorToken();
    Floor testFloor = Floor(number,seed,firstToken);
    Floor consistencyTestFloor = Floor(number,seed);
    string secondToken = testFloor.floorToken();
    string consistencyToken = consistencyTestFloor.floorToken();
    if (firstToken.compare(consistencyToken) == 0) {
        qDebug() << "Basic Tokens are consistent";
    } else {
        QString result = "Basic Tokens are inconsistent, token 1: " + QString::fromStdString(firstToken) + " token 2: " + QString::fromStdString(consistencyToken);
        qDebug() << result;
    }
   if (firstToken.compare(secondToken) == 0) {
        qDebug() << "Hex Tokens are consistent";
    } else {
        QString result = "Hex Tokens are inconsistent, token 1: " + QString::fromStdString(firstToken) + " token 2: " + QString::fromStdString(secondToken);
        qDebug() << result;
    }
}

Floor::Floor(int number, int seed, bool previouslyGenerated){
    //constructs a floor object
    this->seed = seed; //for Unit test
    this->number = number;
    srand(seed+number);
    Tools::width = (rand() % 4) + 15;
    height = (rand() % 3) + 7;
    vector<int> floorCells;
    //vector<vector<Wall>> *tempConnections* = new vector<;
    connections.resize(height);
    for(int r = 0; r < height; r++){
        connections[r].resize(Tools::width);
        for(int c = 0; c < Tools::width; c++){
            Coordinate cell;
            cell.x = c;
            cell.y = r;
            floorCells.push_back(Tools::getCoordinateKey(cell));
            connections[r][c].down = 0;
            connections[r][c].right = 0;
        }
    }
    generateRooms(floorCells, 8);
    generateDoors();
    if (DEBUG) {
    qDebug() << roomsUnitTest();
    }
    if(!previouslyGenerated){
        generateItems();
        generateNPCs(number);
        try {
        generateLockedDoors();
        } catch (LockedDoorException &e) {
            throw e;
        }
    }
    generateLadders(number == 0);
    if (DEBUG && !previouslyGenerated) {
    qDebug() << roomsUnitTest();
    }
}

Floor::Floor(int number,int seed, string floorToken) : Floor(number,seed,true) {
    //method to generate a floor based on a token, current not functioning
    vector<int> hexOut;
    stringstream sstream;
    for (auto it = floorToken.begin(); it != floorToken.end();it+=2) {
        hexOut.insert(hexOut.begin(),byteHexStringToInt(*it,*(it+1)));
    }
    for (int i : hexOut) {
        sstream << i;
    }
    qDebug() << QString::fromStdString(sstream.str()) << " after decoding";
    int count = 0;
    for (Room& room : rooms) {
        if (hexOut.size() <= 0) {
            qDebug() << "Oh noes the hex string ran out but the rooms didn't";
            return;
        }
        //bool visited = (NEXT_HEX != 0); // does nothing at the moment
        int numItemsInRoom = hexOut.back();
        hexOut.pop_back();
        for (int i =0; i < numItemsInRoom; ++i) { //add items to the room
            room.addItem(hexOut.back());
            hexOut.pop_back();
        }
        int numNPCsInRoom = hexOut.back();
        hexOut.pop_back();
        for (int i =0; i < numNPCsInRoom; ++i) {
            
            shared_ptr<NPC> npc = room.addNPC(hexOut.back(),number, true);
            hexOut.pop_back();
            int numItems = hexOut.back();
            hexOut.pop_back();
            for (int j=0; j < numItems; ++j) {
                npc->addItem(hexOut.back());
                hexOut.pop_back();
            }
        }
        if(hexOut.back()){//door is locked in room
            shared_ptr<Door> d = getOuterLockedDoor(count);
            lockDoor(d);
        }
        hexOut.pop_back();
        count++;
    }
}

string Floor::floorToken() {
    //string represntataion of floor object used for saving to file
    stringstream sstream;
    stringstream instream;
    string token = "";
    int count = 0;
    for (Room& room : rooms) {
        //sstream << room.visited();
        sstream << Tools::intToByteHexString(room.itemsInRoom.size());
        instream << room.itemsInRoom.size();
        for (int items : room.itemsInRoom) {
            sstream << Tools::intToByteHexString(items);
            instream << items;
        }
        sstream << Tools::intToByteHexString(room.npcsInRoom.size());
        instream << room.npcsInRoom.size();
        for (shared_ptr<NPC> npc : room.getNPCs()) {
            sstream << Tools::intToByteHexString(npc->getCode());
            instream << npc->getCode();
            for (int items : npc->getInventory()) {
                sstream << Tools::intToByteHexString(items);
                instream << items;
            }
        }
        sstream << Tools::intToByteHexString(getOuterLockedDoor(count)->locked);
        instream << getOuterLockedDoor(count)->locked;
        count++;
    }
    sstream >> token;
    qDebug() << QString::fromStdString(instream.str()) << " before encoding";
    return token;
}

inline bool Floor::cellOutOfBounds(int x, int y) {
    //returns true if and only if the cell is not inside the floors valid coordinates
    return (x < 0 || x >= Tools::width || y >= height || y < 0);
}

Coordinate Floor::getNextCell(Coordinate coord){
    //gets the next cell to connect to a room when generating rooms
    vector<Coordinate> allowedCells;
    vector<Coordinate>::iterator it;
    //check neighbouring cells
    for(int i = 0; i < 4; i++){
        int x = coord.x + (i-1) * (i+1)%2;
        int y = coord.y + (i-2) * i%2;
        if(!cellOutOfBounds(x, y) && disconnectedCell(x,y)){
            Coordinate foo;
            foo.x = x;
            foo.y = y;
            allowedCells.insert(allowedCells.end(), foo);
        }
    }
    if(allowedCells.size()>0){
        
        it=allowedCells.begin() + (int)(rand() % allowedCells.size());
        return *it;//return random cell to connect to 
    }else{
        //no cells to connect to
        Coordinate noValidCell;
        noValidCell.x = -1;
        return noValidCell;
    }
}

bool Floor::disconnectedCell(int x, int y){
    //method returns true if the cell at coordinates x,y is not connected to any other cell
    //Simplifying the logic here would result in fragmentation errors

    return !((!cellOutOfBounds(x-1, y) && connections[y][x-1].right != 0)
        || (!cellOutOfBounds(x, y-1) && connections[y-1][x].down != 0)
        || (!cellOutOfBounds(x+1, y) && connections[y][x].right != 0)
        || (!cellOutOfBounds(x, y+1) && connections[y][x].down != 0));
}

void Floor::generateRooms(vector<int> unusedCells, int maxRoomSize = 7){
    //generates the rooms in a floor, where rooms are just a group of adjacent cells.
    vector<int> connectingCells;
    unordered_set<int> currentRoom;
    while(!unusedCells.empty()){
        //use every cell in the floor to make rooms
        int roomSize = (int)(rand() % (maxRoomSize - 4)) + 4;
        vector<int>::iterator it;
        it = unusedCells.begin() + (int)(rand() % unusedCells.size());
        //start at a random cell
        connectingCells.push_back(*it);
        currentRoom.insert(currentRoom.end(), *it);//add cell to current room
        unusedCells.erase(it);//delete cell from unused cells
        for(int i = 0; i < roomSize-1; ++i){
            it = connectingCells.begin() + (int)(rand() % connectingCells.size());
            Coordinate nextCell = getNextCell(Tools::getKeyCoordinate(*it));//get next cell to connect to
            if(nextCell.x == -1){
                connectingCells.erase(it);
                if(connectingCells.empty()){
                    //no more possible cells to connect to.
                    break;
                }
            }else{

                connectCells(Tools::getKeyCoordinate(*it), nextCell);//connect the cells
                if (i>=2) { //find cells in room next to each other and connect them (ie. delete inner walls in the room)
                    Coordinate tempCell;
                    for (int k = 0; k < 4; k++) {
                        tempCell.x = nextCell.x + (k-1) * (k+1)%2; // -1 0 1 0
                        tempCell.y = nextCell.y + (k-2) * k%2;     // 0 -1 0 1
                        if (!cellOutOfBounds(tempCell.x, tempCell.y) && currentRoom.find(Tools::getCoordinateKey(tempCell)) != currentRoom.end()) {
                            connectCells(tempCell,nextCell);
                        }
                    }
                }
                connectingCells.push_back(Tools::getCoordinateKey(nextCell));
                currentRoom.insert(Tools::getCoordinateKey(nextCell));
                unusedCells.erase(lower_bound(unusedCells.begin(), unusedCells.end(), Tools::getCoordinateKey(nextCell)));
            }
        }
        rooms.push_back(Room(currentRoom));//add room to rooms
        connectingCells.clear();//reset connecting cells for next room  
        currentRoom.clear();//reset current room for next room
    }
}

void Floor::generateDoors(){
    //connects all the rooms in the floor with doors
    vector<Room>::iterator it;
    vector<int> connectedCells;//cells that are connected excluding cells which have no more cells to connect to
    vector<int> deadCells;//cells that are connected
    it = rooms.begin() + (int)(rand() % rooms.size());
    Room r = *it;
    for(int cell : r.cells){
        connectedCells.insert(lower_bound(connectedCells.begin(),connectedCells.end(),cell),cell);
        deadCells.insert(lower_bound(deadCells.begin(),deadCells.end(),cell),cell);
    }
    int count = rooms.size()-1;//connect every room
    while(count > 0){
        vector<int>::iterator it2;
        bool cellNotFound = true;//represents the next cell which will expand connecting cells
        Coordinate outerCell, innerCell;//outer cell is the cell which connecting cells expands to
        while(cellNotFound){
            it2 = connectedCells.begin() + (int)(rand() % connectedCells.size());
            int c = rand() % 4;
            innerCell = Tools::getKeyCoordinate(*it2);//the inner cell which will propogate to the next cell to connect to
            for(int i = c; i < c+4 && cellNotFound; i++){
                //check neighbouring cells of inner cells
                int j = i%4;
                outerCell.x = innerCell.x + (j-1) * (j+1)%2;
                outerCell.y = innerCell.y + (j-2) * j%2;
                if(!cellOutOfBounds(outerCell.x, outerCell.y) && !binary_search(deadCells.begin(),deadCells.end(),Tools::getCoordinateKey(outerCell))){
                    cellNotFound = false;
                    //found a cell to propogate to
                }else{
                    if(i == c +3){
                        //all surrounding cells are either already connected, or else out of bounds.
                        connectedCells.erase(lower_bound(connectedCells.begin(),connectedCells.end(),*it2));
                        //erase the cell if the cell is inside dead cells as it's not usable for room propagation
                    }
                }
            }
        }
        try { //getRoom method throws exception on the rare occasion
        Room &innerRoom = getRoom(Tools::getCoordinateKey(innerCell)); //room you're currently generating the door from
        Room &outerRoom = getRoom(Tools::getCoordinateKey(outerCell)); //the room the door will point to
        for(int cell : outerRoom.cells){ //goes through every cell in the room the door points to
            deadCells.insert(lower_bound(deadCells.begin(),deadCells.end(),cell),cell);
            connectedCells.insert(lower_bound(connectedCells.begin(),connectedCells.end(),cell),cell);
        }
        connectRooms(innerRoom, outerRoom, innerCell, outerCell);
        count--;
        } catch (GetRoomException &e) { //is thrown if getRoom fails, however it should never fail but if it does not return a valid room the result will be discarded and execution will just continue
            continue;
        }
    }    
}

void Floor::generateNPCs(int floorNumber){
    //adds NPCs to the floor
    int numberOfNPCs = (rooms.size() / 4);
    numberOfNPCs *= 1 + ((rand()%11)-5)/float(50);
    for(int i = 0; i< numberOfNPCs; i++){
        vector<Room>::iterator it;
        it = rooms.begin() + (int)(rand() % rooms.size());
        int key = rand() % NUM_HUMANS;
        if(it->getNPCs().size() + 1 < it->getCells().size()){
            it->addNPC(key, floorNumber);
        }else{
            i--;
        }
    }
}

void Floor::generateLockedDoors(){
    //Average number of 1 door rooms per floor is 8.6
    //locks some of the doors in the room
    vector<Room*> lockedRooms;
    for(Room &r : rooms){
        if(r.getDoors().size() == 1 && (rand() % 20 < 3)){
            lockedRooms.push_back(&r);
        }
    }
    for(Room *r : lockedRooms){
        try{
            shared_ptr<Door> d = getOuterLockedDoor(getRoomIndex(*r));
            lockDoor(d);
        }catch(LockedDoorException &e){ //is thrown if getOuterLockedDoor fails and is thrown upward as it completely breaks room generation (should never occur however)
            throw e;
        }
        //place a key for that locked room in a different room in the floor
        auto keyRoom = (rooms.begin() + (int)(rand() % rooms.size()));
        while(keyRoom->getDoors().size() < 2){
            keyRoom = (rooms.begin() + (int)(rand() % rooms.size()));
        }
        if(keyRoom->getNPCs().size() > 0){
            //give key to npc
            int likedItem = 0;
            auto likedItemRoom = rooms.begin();
            shared_ptr<NPC> npc = dynamic_pointer_cast<NPC>(*(keyRoom->getNPCs().begin() + (int)(rand() % keyRoom->getNPCs().size())));
            npc->giveKey();
            likedItem = npc->getLikedItem();
            likedItemRoom = (rooms.begin() + (int)(rand() % rooms.size()));            
            while((int)*likedItemRoom == ((int)*keyRoom) || likedItemRoom->getDoors().size() < 2){
                likedItemRoom = (rooms.begin() + (int)(rand() % rooms.size()));
            }
            likedItemRoom->addItem(likedItem);//add item the npc likes to a random room
        }else{
            keyRoom->addItem(0);//add key to room
        }
    }

}

void Floor::generateItems(){
    //adds items to the floor
    int numberOfItems = (rooms.size() / 2);
    numberOfItems *= 1 + ((rand()%11)-5)/50;
    for(int i = 0; i < numberOfItems; i++){
        auto itemRoom = (rooms.begin() + (int)(rand() % rooms.size()));
        int rarity = rand() % 7;
        rarity /= 2;
        rarity = max(rarity, 1);
        if(itemRoom->getDoors().size() == 1){
            rarity += 1;
        }
        vector<short> possibleItems = Item::itemRarity.at(rarity);
        itemRoom->addItem(possibleItems.at(rand() % possibleItems.size()));
    }
}

int Floor::getRoomIndex(Room& r){
    //gets the index of a room
    vector<Room>::iterator it = rooms.begin();
    int count = 0;
    while(it != rooms.end()){
        if((*it) == r){
            return count;
        }
        count++;
        it++;
    }
    qDebug() << "returning index -1";
    return -1;
}

void Floor::generateLadders(bool firstFloor = false){
    //generates the "objects" that toggle between floors
    auto upRoom = (rooms.begin() + (int)(rand() % rooms.size()));
    upRoom->giveLadder(true);
    upRoom->hasKiosk = true;
    vector<Room>::iterator it = (rooms.begin() + (int)(rand() % rooms.size()));
    auto downRoom = it;
    while(downRoom->getDoors().size() < 2 && (int)*downRoom == (int)*upRoom){
        it = (rooms.begin() + (int)(rand() % rooms.size()));
        downRoom = it;
    }
    //down button is generated but not shown to user as game saving is not fully implemented yet
    downRoomIndex = getRoomIndex(*downRoom);
    if(!firstFloor){
        downRoom->giveLadder(false);
    }
}

Room& Floor::getRoom(int cellKey){
    //gets the room that contains a given cell.
    for(Room &r : rooms){
        if(r.cells.find(cellKey) != r.cells.end()){
            return r;
        }
    } 
    throw GetRoomException();
}

void Floor::connectRooms(Room &r1, Room &r2, Coordinate c1, Coordinate c2){
    //connects two rooms with a door where c1 represents the doors adjacent coordinate in r1, and c2 does the same with r2.
    Door d;
    if(c1.x == c2.x){
        if(c1.y < c2.y){
            connections[c1.y][c1.x].down = 2;
            d.doorLocation = c1;
        }else{
            connections[c2.y][c2.x].down = 2;
            d.doorLocation = c2;
        }
        d.vertical = false;
    }else{
        if(c1.x < c2.x){
            connections[c1.y][c1.x].right = 2;
            d.doorLocation = c1;
        }else{
            connections[c2.y][c2.x].right = 2;
            d.doorLocation = c2;
        }
        d.vertical = true;
    }
    d.locked = false;
    vector<Room>::iterator it = rooms.begin();
    Door d2 = Door(d);
    int count = 0;
    while(it != rooms.end()){
        if((*it) == r2){
            d.roomIndex = count;
        }else if((*it) == r1){
            d2.roomIndex = count;
        }
        count++;
        it++;
    }
    r1.addDoor(d);
    r2.addDoor(d2);
}

void Floor::connectCells(Coordinate c1, Coordinate c2){
    //makes two adjacent cells "connected", ie. they are in the same room
    if(c1.x == c2.x){
        if(c1.y < c2.y){
            connections[c1.y][c1.x].down = 1;
        }else{
            connections[c2.y][c2.x].down = 1;
        }
    }else{
        if(c1.x < c2.x){
            connections[c1.y][c1.x].right = 1;
        }else{
            connections[c2.y][c2.x].right = 1;
        }
    }
}

int Floor::getWidth(){
    return Tools::width;
}

int Floor::getHeight(){
    return height;
}

vector<vector<Wall>>& Floor::getConnections(){
    return connections;
}
