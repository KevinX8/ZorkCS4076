#include "MapWidget.h"

MapWidget::MapWidget() : f(0, 587){
    f = Floor(0, 587);
    this->setFixedSize(600,500);
}

void MapWidget::paintEvent(QPaintEvent *event){
    QPainter p;
    p.begin(this);
    vector<vector<Wall>> connections;
    connections = f.getConnections();
    for(int r = 0; r < f.getHeight(); r++){
        for(int c = 0; c < f.getWidth(); c++){
            Coordinate topRight, bottomRight, bottomLeft;
            bottomRight.x = topRight.x = 50*(c+1);
            topRight.y = 50*(r);
            bottomLeft.y = bottomRight.y = topRight.y + 50;
            bottomLeft.x = topRight.x - 50;
            if(connections[r][c].right == 0){
                p.drawLine(topRight.x,topRight.y, bottomRight.x, bottomRight.y);
            }
            if(connections[r][c].down == 0){
                p.drawLine(bottomLeft.x,bottomLeft.y, bottomRight.x, bottomRight.y);
            } 
        }
    }
}
