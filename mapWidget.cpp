#include "MapWidget.h"
#include <ctime>

MapWidget::MapWidget() : f(0, time(nullptr)){
    this->setFixedSize(f.getWidth()*75+4,f.getHeight()*75+4);
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
    QPen pen(Qt::black, 2, Qt::SolidLine);
    qp->setPen(pen);

    qp -> drawLine(1,1,1,f.getHeight()*scale+1);
    qp -> drawLine(1,1,f.getWidth()*scale+1,1);
    for(int r = 0; r < f.getHeight(); r++){
        for(int c = 0; c < f.getWidth(); c++){
            Coordinate topRight, bottomLeft;
            topRight.x = scale*(c+1)+1;
            topRight.y = scale*(r)+1;
            bottomLeft.y = topRight.y + scale;
            bottomLeft.x = topRight.x - scale;
            if(connections[r][c].right == 0){
                qp->drawLine(topRight.x,topRight.y, topRight.x, bottomLeft.y);
            }
            if(connections[r][c].down == 0){
                qp->drawLine(bottomLeft.x,bottomLeft.y, topRight.x, bottomLeft.y);
            }
        }
    }
}
