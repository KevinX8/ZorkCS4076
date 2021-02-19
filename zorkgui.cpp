#include "zorkgui.h"
#include "ui_zorkgui.h"
#include "Floor.h"

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

ZorkGUI::drawFloow(Floor *floor){
    QPainter p;
    p.begin(this);
    for(int i = 0; i < )
}