#ifndef __INVENTORYWIDGET_H__
#define __INVENTORYWIDGET_H__
#include <QList>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QListView>
#include <QWidget>
#include "../interaction/Player.h"

class GameInstance;

class InventoryWidget : public QWidget {
    Q_OBJECT
    private:
    friend class Player;
    Player* player;
    GameInstance &instance;
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
    InventoryWidget(Player* player, GameInstance instance);
    void updateStats();
    void updateEquipment();
    void updateInventory();
    void changeDisplay(bool showInventory = true);
    void unEquip(int type);
    void equip(shared_ptr<Item> itemToEquip, int slot);
};

#endif // __INVENTORYWIDGET_H__
