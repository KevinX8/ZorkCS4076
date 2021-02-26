#include "MapWidget.h"
#include <ctime>

MapWidget::MapWidget() : f(0, time(nullptr)){
    this->setFixedSize(f.getWidth()*75+10,f.getHeight()*75+10);
}

void MapWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

      QPainter qp(this);
      drawWalls(&qp);
}

void MapWidget::drawWalls(QPainter *qp){
    vector<vector<Wall>> connections;
    connections = f.getConnections();
    int scale = 75;
    int wallWidth = 10;
    QPen pen(Qt::black, 2, Qt::SolidLine);
    QBrush wallBrush(Qt::Dense3Pattern);
    QBrush doorBrush(Qt::Dense6Pattern);
    qp->setPen(pen);

    qp -> fillRect(0,0, scale*f.getWidth(),wallWidth, wallBrush);
    qp -> fillRect(0,0, wallWidth,scale*f.getHeight(), wallBrush);
    for(int r = 0; r < f.getHeight(); r++){
        for(int c = 0; c < f.getWidth(); c++){
            Coordinate topRight, bottomLeft;
            topRight.x = scale*(c+1)+wallWidth/2;
            topRight.y = scale*(r)+wallWidth/2;
            bottomLeft.y = topRight.y + scale;
            bottomLeft.x = topRight.x - scale;
            switch(connections[r][c].right){
                case(0):{
                    qp->fillRect(topRight.x-(wallWidth/2),topRight.y-(wallWidth/2), wallWidth,scale+wallWidth, wallBrush);
                    break;
                }
                case(2):{
                    qp->fillRect(topRight.x-(wallWidth/2),topRight.y+(wallWidth/2), wallWidth,scale-wallWidth, doorBrush);
                }
            }
            switch(connections[r][c].down){
                case(0):{
                    qp->fillRect(bottomLeft.x-(wallWidth/2),bottomLeft.y-(wallWidth/2), scale+wallWidth,wallWidth, wallBrush);
                    break;
                }
                case(2):{
                    qp->fillRect(bottomLeft.x+(wallWidth/2),bottomLeft.y-(wallWidth/2), scale-wallWidth,wallWidth, doorBrush);
                }
            }
        }
    }
}
