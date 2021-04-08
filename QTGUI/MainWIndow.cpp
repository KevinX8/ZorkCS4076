#include "MainWindow.h"

MainWindow::MainWindow(shared_ptr<RoomItemWidget> r, shared_ptr<MapWidget> m, shared_ptr<TextBoxWidget> t, shared_ptr<InventoryWidget> i){
    this->map = m;
    this-> inv = i;
    this-> room = r;
    this-> text = t;
    map->setParent(this);
    inv->setParent(this);
    room->setParent(this);
    text->setParent(this);
    setCentralWidget(map.get());
    resize(WINDOW_W,WINDOW_H);
    map->setGeometry(MAP_X,MAP_Y-30,MAP_W,MAP_H+30);
    inv->setGeometry(PLAYERINV_X,INV_Y-30,INV_W,INV_H+30);
    inv->rightInventoryView->setGeometry(0,30,INV_W,INV_H);
    inv->rightEquipmentView->setGeometry(0,30,INV_W,INV_H);
    inv->changeInvButton->setGeometry(0,0,30,30);
    inv->inventoryType->setGeometry(INV_W-110,0,110,30);
    inv->playerStats->move(INV_W/2-inv->playerStats->width(),0);
    room->setGeometry(ROOMINV_X,INV_Y-30,INV_W,INV_H+30);
    room->listView->setGeometry(0,30,INV_W,INV_H);
    text->setGeometry(TEXTBOX_X,TEXTBOX_Y,TEXTBOX_W,TEXTBOX_H);
    text->textBox->resize(TEXTBOX_W,TEXTBOX_H);
}

MainWindow::MainWindow(){

}
