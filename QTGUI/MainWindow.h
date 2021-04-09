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

#define ROOMINV_X 5 * RESSCALE
#define PLAYERINV_X 940 * RESSCALE
#define INV_W 340 * RESSCALE
#define INV_H 220 * RESSCALE
#define INV_Y 500 * RESSCALE

#define TEXTBOX_X INV_W + 7
#define TEXTBOX_Y INV_Y //should be the same as INV_Y maybe?
#define TEXTBOX_W PLAYERINV_X - INV_W -10
#define TEXTBOX_H INV_H //should be same as INV_H probably
//horizontal centre align text and add autofill

namespace QTGui {
    class MainWindow;
}

using namespace QTGui;

class QTGui::MainWindow : public QMainWindow {
  Q_OBJECT
  private:
    friend class GameInstance;
    shared_ptr<MapWidget> map;
    shared_ptr<InventoryWidget> inv;
    shared_ptr<RoomItemWidget> room;
    shared_ptr<TextBoxWidget> text;
  public:
    MainWindow();
    MainWindow(shared_ptr<RoomItemWidget> roomWidget,
    shared_ptr<MapWidget> map,
    shared_ptr<TextBoxWidget> text,
    shared_ptr<InventoryWidget> inv
    );
    void changeMapWidget(shared_ptr<MapWidget> m);

};

#endif
