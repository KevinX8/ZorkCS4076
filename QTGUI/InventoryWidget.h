#ifndef __INVENTORYWIDGET_H__
#define __INVENTORYWIDGET_H__
#include <QList>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QListView>
#include <QWidget>
#include <QPushButton>
#include <functional>
#include <string>
#include <QPushButton>
#include "../interaction/Player.h"
class InventoryWidget : public QWidget {
    Q_OBJECT
    private:
    Player* player;
    std::function<void(shared_ptr<Item>)> dropFunc;
    QList<QAction*> playerItems;
    QList<QAction*> equipedItems;
    unique_ptr<QMenu> wearableWeaponSubMenu;
    unique_ptr<QMenu> itemMenu;
    unique_ptr<QAction> use;
    unique_ptr<QAction> asWeapon;
    unique_ptr<QAction> asWearable;
    unique_ptr<QAction> drop;
    unique_ptr<QAction> close;
    void setItemInteraction(shared_ptr<Item> item);
    public:
    unique_ptr<QPushButton> changeInvButton;
    unique_ptr<QLabel> playerStats;
    unique_ptr<QLabel> inventoryType;
    QListView rightInventoryView;
    QListView rightEquipmentView;
    InventoryWidget(Player* player, std::function<void(shared_ptr<Item>)> dropFunc);
    void updateStats();
    void updateEquipment(int type);
    void updateInventory(int index);
    void changeDisplay();
    void unEquip(int type);
    void equip(shared_ptr<Item> itemToEquip, int slot);
};

#endif // __INVENTORYWIDGET_H__
