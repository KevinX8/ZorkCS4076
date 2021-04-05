#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <QWidget>
#include "InventoryWidget.h"
#include "MapWidget.h"
#include "RoomItemWidget.h"
#include "TextBoxWidget.h"

class MainWindow {
  private:
    friend class GameInstance;
    MapWidget map;
    InventoryWidget inv;
    RoomItemWidget room;
    TextBoxWidget text;
  public:
    MainWindow();
};

#endif
