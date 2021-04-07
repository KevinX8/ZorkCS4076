#include "MainWindow.h"

MainWindow::MainWindow(/*shared_ptr<RoomItemWidget> r,*/ shared_ptr<MapWidget> m/*, shared_ptr<TextBoxWidget> t, shared_ptr<InventoryWidget> i*/){
    this->map = m;
    /*
    this-> inv = i;
    this-> room = r;
    this-> text = t;
    */
    setCentralWidget(map.get());
    resize(WINDOW_W,WINDOW_H);
}

MainWindow::MainWindow(){

}