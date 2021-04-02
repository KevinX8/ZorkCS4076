#ifndef __INVENTORYWIDGET_H__
#define __INVENTORYWIDGET_H__
#include "..\GameWorld\GameInstance.h"
#include <QList>
#include <vector>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QListView>

class InventoryWidget : public QWidget {
    Q_OBJECT
    private:
    friend class Player;
    Player* player;
    QList<QAction> playerItems;
    QList<QAction> equipedItems;
    QLabel playerStats;
    QMenu wearableWeaponSubMenu;
    QMenu itemMenu;
    QAction use;
    void setItemInteraction(int itemCode);
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