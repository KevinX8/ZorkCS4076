#include "MapWidget.h"
#include <ctime>

#define SCALE 75
#define WALL_WIDTH 10

MapWidget::MapWidget(Floor& floor, Room& r, std::function<void(Door&)> dF, std::function<void(NPC&)> nF, std::function<void()> rF, std::function<void(bool)> cF, QWidget *parent) :QWidget(parent), current(r), f(floor){
    this->resetButtons();
    this->doorFunc = dF;
    this->npcFunc = nF;
    this->resetFunc = rF;
    this->floorFunc = cF;
}

void MapWidget::resetButtons(){
    resetFunc();
    npcButtons.clear();
    doorButtons.clear();
    unordered_set<int> openCells = current.getCells();
    auto it = openCells.begin();
    for(shared_ptr<NPC> n : current.getNPCs()){
        Coordinate c = Tools::getKeyCoordinate((*it));
        it++;
        shared_ptr<QPushButton> button = (n->getCode() < NUM_HUMANS)? shared_ptr<QPushButton>(new QPushButton("👤", this)): shared_ptr<QPushButton>(new QPushButton("🐕", this));
        button->setGeometry(c.x * SCALE,c.y * SCALE,SCALE,SCALE);
        connect(button.get(), &QPushButton::released, this, [n](){MapWidget::npcFunc(*n);});
        npcButtons.push_back(button);
    }
    bool hasUp = current.containsUpLadder();
    bool hasDown = current.containsDownLadder();
    if(hasUp || hasDown){
        QString icon = (hasUp)? "↗️" : "↘️";
        Coordinate c = Tools::getKeyCoordinate((*it));
        shared_ptr<QPushButton> button = shared_ptr<QPushButton>(new QPushButton(icon, this));
        button->setGeometry(c.x * SCALE,c.y * SCALE,SCALE,SCALE);
        connect(button.get(), &QPushButton::released, this , [hasUp](){MapWidget::floorFunc(hasUp);});
        npcButtons.push_back(button);
    }
    for(Door& d : current.getDoors()){
        //make door button
        shared_ptr<QPushButton> button = (d.locked)? shared_ptr<QPushButton>(new QPushButton("🔒", this)) : shared_ptr<QPushButton>(new QPushButton("⚫", this));
        Coordinate topRight, bottomLeft;
        topRight.x = SCALE*(d.doorLocation.x+1)+WALL_WIDTH/2;
        topRight.y = SCALE*(d.doorLocation.y)+WALL_WIDTH/2;
        bottomLeft.y = topRight.y + SCALE;
        bottomLeft.x = topRight.x - SCALE;

        if(d.vertical){
            button->setGeometry(topRight.x-(WALL_WIDTH/2),topRight.y-(WALL_WIDTH/2), WALL_WIDTH,SCALE+WALL_WIDTH);
        }else{
            button->setGeometry(bottomLeft.x+(WALL_WIDTH/2),bottomLeft.y-(WALL_WIDTH/2), SCALE-WALL_WIDTH,WALL_WIDTH);
        }
        shared_ptr<Door> a = shared_ptr<Door>(new Door(d));
        connect(button.get(), &QPushButton::released, this , [a](){MapWidget::doorFunc(*a);});
    }

}

void MapWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter qp(this);
    drawWalls(&qp);
}

void MapWidget::changeRoom(Room& room){
    current = room;
    resetButtons();
}

void MapWidget::removeNPC(shared_ptr<NPC> npc){
    vector<shared_ptr<NPC>>& npcs = current.getNPCs();

    for(auto it = npcs.begin(); it != npcs.end(); ++it){
        if((*it) == npc){
            npcs.erase(it);
            break;
        }
    }
    resetButtons();
}

void MapWidget::drawWalls(QPainter *qp){
    vector<vector<Wall>> connections;
    connections = f.getConnections();
    QPen pen(Qt::black, 2, Qt::SolidLine);
    QBrush wallBrush(Qt::SolidPattern);
    qp->setPen(pen);

    qp -> fillRect(0,0, SCALE*f.getWidth(),WALL_WIDTH, wallBrush);
    qp -> fillRect(0,0, WALL_WIDTH,SCALE*f.getHeight(), wallBrush);
    for(int r = 0; r < f.getHeight(); r++){
        for(int c = 0; c < f.getWidth(); c++){
            Coordinate topRight, bottomLeft;
            topRight.x = SCALE*(c+1)+WALL_WIDTH/2;
            topRight.y = SCALE*(r)+WALL_WIDTH/2;
            bottomLeft.y = topRight.y + SCALE;
            bottomLeft.x = topRight.x - SCALE;
            switch(connections[r][c].right){
                case(0):{
                    qp->fillRect(topRight.x-(WALL_WIDTH/2),topRight.y-(WALL_WIDTH/2), WALL_WIDTH,SCALE+WALL_WIDTH, wallBrush);
                    break;
                }
            }
            switch(connections[r][c].down){
                case(0):{
                    qp->fillRect(bottomLeft.x-(WALL_WIDTH/2),bottomLeft.y-(WALL_WIDTH/2), SCALE+WALL_WIDTH,WALL_WIDTH, wallBrush);
                    break;
                }
            }
        }
    }
}
