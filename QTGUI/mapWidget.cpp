#include "MapWidget.h"
#include <ctime>

#define SCALE 75;

MapWidget::MapWidget(int startIndex, GameInstance& g, QObject *parent) :QGraphicsScene(parent), f(0, 19), current(f.rooms.at(startIndex)){
    this->resetButtons();

}

void MapWidget::resetButtons(){
    game.resetButtons();
    npcButtons.clear();
    vector<int>::iterator it = current.getCells().begin();
    for(shared_ptr<NPC> n : current.getNPCs()){
        Coordinate c = Tools::getKeyCoordinate((*it));
        it++;
        QPushButton button = QPushButton("NPC", this);
        button->setGeometry(c.x * SCALE,c.y * SCALE,SCALE,SCALE);
        npcButtons.push_back(button);
        connect(button, QPushButton::released, this, game.interactNPC(n));
    }
}

void MapWidget::paintEvent(QPaintEvent *event){
    /*
    Q_UNUSED(event);
      QPainter qp(this);
      drawWalls(&qp);
      */
}

void MapWidget::changeRoom(int roomIndex){
    
}

void MapWidget::drawWalls(QPainter *qp){
    vector<vector<Wall>> connections;
    connections = f.getConnections();
    int wallWidth = 10;
    QPen pen(Qt::black, 2, Qt::SolidLine);
    QBrush wallBrush(Qt::SolidPattern);
    QBrush doorBrush(Qt::Dense6Pattern);
    QBrush lockedDoorBrush(Qt::Dense3Pattern);
    qp->setPen(pen);

    qp -> fillRect(0,0, SCALE*f.getWidth(),wallWidth, wallBrush);
    qp -> fillRect(0,0, wallWidth,SCALE*f.getHeight(), wallBrush);
    for(int r = 0; r < f.getHeight(); r++){
        for(int c = 0; c < f.getWidth(); c++){
            Coordinate topRight, bottomLeft;
            topRight.x = SCALE*(c+1)+wallWidth/2;
            topRight.y = SCALE*(r)+wallWidth/2;
            bottomLeft.y = topRight.y + SCALE;
            bottomLeft.x = topRight.x - SCALE;
            switch(connections[r][c].right){
                case(0):{
                    qp->fillRect(topRight.x-(wallWidth/2),topRight.y-(wallWidth/2), wallWidth,SCALE+wallWidth, wallBrush);
                    break;
                }
                case(2):{
                    qp->fillRect(topRight.x-(wallWidth/2),topRight.y+(wallWidth/2), wallWidth,SCALE-wallWidth, doorBrush);
                    break;
                }
                case(3):{
                    qp->fillRect(topRight.x-(wallWidth/2),topRight.y+(wallWidth/2), wallWidth,SCALE-wallWidth, lockedDoorBrush);
                    break;
                }
            }
            switch(connections[r][c].down){
                case(0):{
                    qp->fillRect(bottomLeft.x-(wallWidth/2),bottomLeft.y-(wallWidth/2), SCALE+wallWidth,wallWidth, wallBrush);
                    break;
                }
                case(2):{
                    qp->fillRect(bottomLeft.x+(wallWidth/2),bottomLeft.y-(wallWidth/2), SCALE-wallWidth,wallWidth, doorBrush);
                    break;
                }
                case(3):{
                    qp->fillRect(bottomLeft.x+(wallWidth/2),bottomLeft.y-(wallWidth/2), SCALE-wallWidth,wallWidth, lockedDoorBrush);
                    break;
                }
            }
        }
    }
}
