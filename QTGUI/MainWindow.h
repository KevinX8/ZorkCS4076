#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "InventoryWidget.h"
#include "MapWidget.h"
#include "RoomItemWidget.h"
#include "TextBoxWidget.h"
#include "..\GameWorld\GameInstance.h"

class MainWindow {
  private:
    friend class GameInstance;
    MapWidget map;
    InventoryWidget inv;
    RoomItemWidget room;
    TextBoxWidget text;
  public:
};

#endif