#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <QWidget>
#include <memory>
#include <QMainWindow>
#include "InventoryWidget.h"
#include "MapWidget.h"
#include "RoomItemWidget.h"
#include "TextBoxWidget.h"

#define RESSCALE 1.5
#define WINDOW_W 1280 * RESSCALE
#define WINDOW_H 720 * RESSCALE

#define MAP_X 0 * RESSCALE
#define MAP_Y 0 * RESSCALE
#define MAP_W 1280 * RESSCALE
#define MAP_H 470 * RESSCALE

#define ROOMLABEL_X 120 * RESSCALE
#define LABEL_Y 470 * RESSCALE
#define ROOMLABEL_W 110 * RESSCALE
#define ROOMLABEL_H 16 * RESSCALE

#define STATSLABEL_X 550 * RESSCALE
#define STATSLABEL_W 220 * RESSCALE
#define STATSLABEL_H 21 * RESSCALE

#define INVLABEL_X 1080 * RESSCALE
#define INVLABEL_W 100 * RESSCALE
#define INVLABEL_H 16 * RESSCALE

#define TEXTBOX_X 350 * RESSCALE
#define TEXTBOX_Y 490 * RESSCALE //should be the same as INV_Y maybe?
#define TEXTBOX_W 600 * RESSCALE
#define TEXTBOX_H 220 * RESSCALE //should be same as INV_H probably
//horizontal centre align text and add autofill background

#define BUTTON_Y 640 * RESSCALE
#define BUTTON1_X 370 * RESSCALE
#define BUTTON2_X 520 * RESSCALE
#define BUTTON3_X 670 * RESSCALE
#define BUTTON4_X 820 * RESSCALE
#define FUNCBUTTON_W 120 * RESSCALE//width and height of every button is the same
#define FUNCBUTTON_H 30 * RESSCALE
#define BACKPACKBUTTON_X 930 * RESSCALE
#define BACKPACKBUTTON_Y 490 * RESSCALE
#define BACKPACKBUTTON_W 40 * RESSCALE
#define BACKPACKBUTTON_H 30 * RESSCALE

#define ROOMINV_X 0 * RESSCALE
#define PLAYERINV_X 970 * RESSCALE
#define INV_Y 470 * RESSCALE
#define INV_W 340 * RESSCALE
#define INV_H 220 * RESSCALE

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
