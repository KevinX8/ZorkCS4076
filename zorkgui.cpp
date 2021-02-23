#include "zorkgui.h"
#include "ui_zorkgui.h"
#include "Floor.h"

ZorkGUI::ZorkGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZorkGUI)
{
    ui->setupUi(this);
    //Floor *f = new Floor(0, 17898233);
    //QMainWindow::show(this, f);
}

ZorkGUI::~ZorkGUI()
{
    delete ui;
}


void ZorkGUI::paintEvent(QPaintEvent *, Floor *f){
    QPainter p;
    p.begin(this);
    vector<vector<Wall>> connections;
    connections = f->getConnections();
    for(int r = 0; r < f->getHeight(); r++){
        for(int c = 0; c < f->getWidth(); c++){
            Coordinate topRight, bottomRight, bottomLeft;
            bottomRight.x = topRight.x = 50*(c+1);
            topRight.y = 50*(r);
            bottomLeft.y = bottomRight.y = topRight.y + 50;
            bottomLeft.x = topRight.x - 50;
            if(connections[r][c].right == 0){

                p.drawLine(topRight.x,topRight.y, bottomRight.x, bottomRight.y);
            }
        }
    }
}
