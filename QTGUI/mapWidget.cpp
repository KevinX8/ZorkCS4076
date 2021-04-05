#include "MapWidget.h"
#include <ctime>

#define SCALE 75;
#define WALL_WIDTH 10;

MapWidget::MapWidget(int startIndex, GameInstance& g, QObject *parent) :QGraphicsScene(parent), f(0, 19), game(g), current(f.rooms.at(startIndex)){
    this->resetButtons();
}

void MapWidget::resetButtons(){
    game.resetButtons();
    npcButtons.clear();
    doorButtons.clear();
    vector<int>::iterator it = current.getCells().begin();
    for(shared_ptr<NPC> n : current.getNPCs()){
        Coordinate c = Tools::getKeyCoordinate((*it));
        it++;
        QPushButton button
        if(n->getCode < NUM_HUMANS){
            button = QPushButton("👤", this);
        }else{
            button = QPushButton("🐕", this);
        }
        button->setGeometry(c.x * SCALE,c.y * SCALE,SCALE,SCALE);
        connect(button, QPushButton::released, this, game.interactNPC(n));
        npcButtons.push_back(button);
    }
    bool hasUp = current.hasUpLadder;
    bool hasDown = current.hasDownLadder;
    if(hasUp || hasDown){
        char icon = (hasUp?) '↗️' : '↘️';
        Coordinate c = Tools::getKeyCoordinate((*it));
        QPushButton button = QPushButton(("" + icon), this);    
        button->setGeometry(c.x * SCALE,c.y * SCALE,SCALE,SCALE);
        connect(button, QPushButton::released, this , game.changeFloor(hasUp));
        npcButtons.push_back(button);
    }
    for(Door& d : current.getDoors()){
        //make door button
        QPushButton button;

        if(d.locked){
            button = QPushButton("🔒", this);
        }else{
            button = QPushButton("⚫", this);
        }

        if(d.vertical){
            button->setGeometry(topRight.x-(WALL_WIDTH/2),topRight.y-(WALL_WIDTH/2), WALL_WIDTH,SCALE+WALL_WIDTH)
        }else{
            button->setGeometry(bottomLeft.x+(WALL_WIDTH/2),bottomLeft.y-(WALL_WIDTH/2), SCALE-WALL_WIDTH,WALL_WIDTH);
        }

        connect(button, QPushButton::released, this , std::bind(&GameInstance::changeRoom, game, d));
    }

}

void MapWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter qp(this);
    drawWalls(&qp);
}

void MapWidget::changeRoom(&Room room){
    current = room;
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
