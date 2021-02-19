#include "zorkgui.h"
#include "ui_zorkgui.h"

ZorkGUI::ZorkGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZorkGUI)
{
    ui->setupUi(this);
    
}

ZorkGUI::~ZorkGUI()
{
    delete ui;
}

/*
void ZorkGUI::drawFloor(Floor *floor){
    QPainter p;
    p.begin(this);
    for(int i = 0; i < )
}
*/
