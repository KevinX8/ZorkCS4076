#include "MapWidget.h"
#include <ctime>

#define SCALE 75
#define WALL_WIDTH 10

MapWidget::MapWidget(int floorNumber, int startRoomIndex, Floor& floor, Room& r, std::function<void(Door&)>& dF, std::function<void(shared_ptr<NPC>)>& nF, std::function<void()>& rF, std::function<void(bool)>& cF, QWidget *parent) :QWidget(parent), current(r), f(floor){
    this->doorFunc = dF;
    this->npcFunc = nF;
    this->resetFunc = rF;
    this->floorFunc = cF;
    this->currentRoomIndex = startRoomIndex;
    this->xOffset = 960 - ((f.getWidth() * SCALE + WALL_WIDTH + WALL_WIDTH / 2) / 2);
    this->resetButtons();
    QString floorString;
    if(floorNumber % 100 > 9 && floorNumber % 100 < 20){
        floorString = QString::fromStdString((to_string(floorNumber) + "th Floor"));
    }else if(floorNumber == 0){
        floorString = QString::fromStdString("Ground Floor");
    }else{
        switch (floorNumber % 10){
            case(1):{
                floorString = QString::fromStdString((to_string(floorNumber) + "st Floor"));
                break;
            }
            case(2):{
                floorString = QString::fromStdString((to_string(floorNumber) + "nd Floor"));
                break;
            }
            case(3):{
                floorString = QString::fromStdString((to_string(floorNumber) + "rd Floor"));
                break;
            }
            default:{
                floorString = QString::fromStdString((to_string(floorNumber) + "th Floor"));
                break;
            }

        }
    }
    QFont font( "Arial", 15, QFont::Bold);
    floorNumberLabel = unique_ptr<QLabel>(new QLabel(floorString,this));
    floorNumberLabel->setFont(font);
    floorNumberLabel->move(960-floorNumberLabel->width()/2,750 - floorNumberLabel->height()-5);
}

void MapWidget::resetButtons(){
    npcButtons.clear();
    doorButtons.clear();
    unordered_set<int> openCells = current.getCells();
    auto it = openCells.begin();
    for(shared_ptr<NPC> n : current.getNPCs()){
        Coordinate c = Tools::getKeyCoordinate((*it));
        it++;
        shared_ptr<QPushButton> button = (n->getCode() < NUM_HUMANS)? shared_ptr<QPushButton>(new QPushButton("👤", this)): shared_ptr<QPushButton>(new QPushButton("🐕", this));
        button->setGeometry(c.x * SCALE + WALL_WIDTH + xOffset,c.y * SCALE+ WALL_WIDTH,SCALE- WALL_WIDTH,SCALE- WALL_WIDTH);
        connect(button.get(), &QPushButton::released, this, [this,n](){MapWidget::npcFunc(n);});
        npcButtons.push_back(button);
        button->show();
    }
    bool hasUp = current.containsUpLadder();
    bool hasDown = false;
    if(hasUp || hasDown){
        QString icon = (hasUp)? "⬆️" : "⬇️";
        Coordinate c = Tools::getKeyCoordinate((*it));
        shared_ptr<QPushButton> button = shared_ptr<QPushButton>(new QPushButton(icon, this));
        button->setGeometry(c.x * SCALE + WALL_WIDTH + xOffset,c.y * SCALE+ WALL_WIDTH,SCALE- WALL_WIDTH,SCALE- WALL_WIDTH);
        connect(button.get(), &QPushButton::released, this , [this,hasUp](){MapWidget::floorFunc(hasUp);});
        npcButtons.push_back(button);
        button->show();
    }
    for(Door& d : f.rooms.at(currentRoomIndex).getDoors()){
        //make door button
        shared_ptr<QPushButton> button = shared_ptr<QPushButton>(new QPushButton("", this));
        Coordinate topRight, bottomLeft;
        QPalette pal = button->palette();
        if (d.locked) {
             pal.setColor(QPalette::Button, QColor(Qt::darkGray));
        } else {
             pal.setColor(QPalette::Button, QColor(Qt::darkRed));
        }
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->setFlat(true);
        button->update();
        topRight.x = SCALE*(d.doorLocation.x+1)+WALL_WIDTH/2;
        topRight.y = SCALE*(d.doorLocation.y)+WALL_WIDTH/2;
        bottomLeft.y = topRight.y + SCALE;
        bottomLeft.x = topRight.x - SCALE;

        if(d.vertical){
            button->setGeometry(topRight.x-(WALL_WIDTH/2) + xOffset,topRight.y+(WALL_WIDTH/2), WALL_WIDTH,SCALE-WALL_WIDTH);
        }else{
            button->setGeometry(bottomLeft.x+(WALL_WIDTH/2) + xOffset,bottomLeft.y-(WALL_WIDTH/2), SCALE-WALL_WIDTH,WALL_WIDTH);
        }
        shared_ptr<Door> a = shared_ptr<Door>(new Door(d));
        connect(button.get(), &QPushButton::released, this , [this,a](){MapWidget::doorFunc(*a);});
        doorButtons.push_back(button);
        button->show();
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
    current.removeNPC(npc);
    f.rooms.at(currentRoomIndex).removeNPC(npc);
    resetButtons();
}

void MapWidget::drawWalls(QPainter *qp){
    vector<vector<Wall>> connections;
    connections = f.getConnections();
    QPen pen(Qt::black, 2, Qt::SolidLine);
    QBrush wallBrush(Qt::SolidPattern);
    QBrush doorBrush(Qt::Dense6Pattern);
    QBrush lockedDoorBrush(Qt::Dense3Pattern);
    qp->setPen(pen);

    qp -> fillRect(xOffset,0, SCALE*f.getWidth(),WALL_WIDTH, wallBrush);
    qp -> fillRect(xOffset,0, WALL_WIDTH,SCALE*f.getHeight(), wallBrush);
    for(int r = 0; r < f.getHeight(); r++){
        for(int c = 0; c < f.getWidth(); c++){
            Coordinate topRight, bottomLeft;
            topRight.x = SCALE*(c+1)+WALL_WIDTH/2;
            topRight.y = SCALE*(r)+WALL_WIDTH/2;
            bottomLeft.y = topRight.y + SCALE;
            bottomLeft.x = topRight.x - SCALE;
            switch(connections[r][c].right){
                case(0):{
                    qp->fillRect(topRight.x-(WALL_WIDTH/2) + xOffset,topRight.y-(WALL_WIDTH/2), WALL_WIDTH,SCALE+WALL_WIDTH, wallBrush);
                    break;
                }
                case(2):{
                    qp->fillRect(topRight.x-(WALL_WIDTH/2) + xOffset,topRight.y-(WALL_WIDTH/2), WALL_WIDTH,SCALE+WALL_WIDTH, doorBrush);;
                    break;
                }
                case(3):{
                    qp->fillRect(topRight.x-(WALL_WIDTH/2) + xOffset,topRight.y-(WALL_WIDTH/2), WALL_WIDTH,SCALE+WALL_WIDTH, doorBrush);
                    break;
                }
            }
            switch(connections[r][c].down){
                case(0):{
                    qp->fillRect(bottomLeft.x-(WALL_WIDTH/2) + xOffset,bottomLeft.y-(WALL_WIDTH/2), SCALE+WALL_WIDTH,WALL_WIDTH, wallBrush);
                    break;
                }
                case(2):{
                    qp->fillRect(bottomLeft.x-(WALL_WIDTH/2) + xOffset,bottomLeft.y-(WALL_WIDTH/2), SCALE+WALL_WIDTH,WALL_WIDTH, doorBrush);
                    break;
                }
                case(3):{
                    qp->fillRect(bottomLeft.x-(WALL_WIDTH/2) + xOffset,bottomLeft.y-(WALL_WIDTH/2), SCALE+WALL_WIDTH,WALL_WIDTH, doorBrush);
                    break;
                }
            }
        }
    }
}
