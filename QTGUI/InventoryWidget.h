#ifndef __INVENTORYWIDGET_H__
#define __INVENTORYWIDGET_H__
#include "..\GameWorld\GameInstance.h"
#include <QList>
#include <vector>
#include <QLabel>

class InventoryWidget {
    private:
    friend class Player;
    Player* player;
    QList<QAction> playerItems;
    QList<QAction> equipedItems;
    QLabel playerStats;
    public:
    QListView rightInventoryView;
    QListView rightEquipmentView;
    InventoryWidget(Player* player);
    void updateStats();
    void updateEquipment();
    void updateInventory();
    void changeDisplay(bool showInventory = true);
}

#endif // __INVENTORYWIDGET_H__