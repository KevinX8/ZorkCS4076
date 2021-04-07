#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <QWidget>
#include <memory>
#include <QMainWindow>
#include "InventoryWidget.h"
#include "MapWidget.h"
#include "RoomItemWidget.h"
#include "TextBoxWidget.h"

#define WINDOW_W 1280
#define WINDOW_H 720

#define MAP_X 0
#define MAP_Y 0
#define MAP_W 1280
#define MAP_H 470

#define ROOMLABEL_X 120
#define LABEL_Y 470
#define ROOMLABEL_W 110
#define ROOMLABEL_H 16

#define STATSLABEL_X 550
#define STATSLABEL_W 220
#define STATSLABEL_H 21

#define INVLABEL_X 1080
#define INVLABEL_W 100
#define INVLABEL_H 16

#define TEXTBOX_X 350
#define TEXTBOX_Y 490 //should be the same as INV_Y maybe?
#define TEXTBOX_W 600
#define TEXTBOX_H 220 //should be same as INV_H probably
//horizontal centre align text and add autofill background

#define BUTTON_Y 640
#define BUTTON1_X 370
#define BUTTON2_X 520
#define BUTTON3_X 670
#define BUTTON4_X 820
#define FUNCBUTTON_W 120//width and height of every button is the same
#define FUNCBUTTON_H 30
#define BACKPACKBUTTON_X 930
#define BACKPACKBUTTON_Y 490
#define BACKPACKBUTTON_W 40
#define BACKPACKBUTTON_H 30

#define ROOMINV_X 0
#define PLAYERINV_X 970
#define INV_Y 470
#define INV_W 340
#define INV_H 220

class MainWindow : public QMainWindow {
  Q_OBJECT
  private:
    friend class GameInstance;
    shared_ptr<MapWidget> map;
    shared_ptr<InventoryWidget> inv;
    shared_ptr<RoomItemWidget> room;
    shared_ptr<TextBoxWidget> text;
  public:
    MainWindow();
    MainWindow(//shared_ptr<RoomItemWidget> roomWidget,
    shared_ptr<MapWidget> map
    //shared_ptr<TextBoxWidget> text,
    //shared_ptr<InventoryWidget> inv
    );

};

#endif
