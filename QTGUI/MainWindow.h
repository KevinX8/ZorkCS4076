#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <QWidget>
#include <memory>
#include "InventoryWidget.h"
#include "MapWidget.h"
#include "RoomItemWidget.h"
#include "TextBoxWidget.h"

#define MAP_X
#define MAP_Y
#define MAP_W
#define MAP_H

#define ROOMLABEL_X
#define ROOMLABEL_Y
#define ROOMLABEL_W
#define ROOMLABEL_H

#define STATSLABEL_X
#define STATSLABEL_Y
#define STATSLABEL_W
#define STATSLABEL_H

#define INVLABEL_X
#define INVLABEL_Y
#define INVLABEL_W
#define INVLABEL_H

#define ROOMINV_X 
#define ROOMINV_Y

#define TEXTBOX_X 
#define TEXTBOX_Y
#define TEXTBOX_W
#define TEXTBOX_H

#define BUTTON1_X 
#define BUTTON1_Y
#define BUTTON2_X 
#define BUTTON2_Y
#define BUTTON3_X 
#define BUTTON3_Y
#define BUTTON4_X 
#define BUTTON4_Y
#define FUNCBUTTON_W
#define FUNCBUTTON_H
#define BACKPACK_X 
#define BACKPACK_Y
#define BACKPACK_W
#define BACKPACK_H

#define PLAYERINV_X 
#define PLAYERINV_Y
#define INV_W
#define INV_H

class MainWindow {
  private:
    friend class GameInstance;
    unique_ptr<MapWidget> map;
    unique_ptr<InventoryWidget> inv;
    unique_ptr<RoomItemWidget> room;
    unique_ptr<TextBoxWidget> text;
  public:
    MainWindow();
};

#endif
