#ifndef __INVENTORYWIDGET_H__
#define __INVENTORYWIDGET_H__
#include <QList>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QPushButton>
#include <functional>
#include <string>
#include "../interaction/Player.h"

#define STRINGEND(type) + " 🎒" + sign(player.type->getInvSpace()) + to_string(player.type->getInvSpace())
#define WEARABLESTRING(type) QString::fromStdString(player.type->getShortDescription() + " 🗣️" + sign(player.type->getCharisma()) + to_string(player.type->getCharisma()) + " ☘️" + to_string(player.type->getLuck()) + STRINGEND(type))

namespace QTGui {
    class InventoryWidget;
}

using namespace QTGui;

class QTGui::InventoryWidget : public QWidget {
    Q_OBJECT
    private:
    Player& player;
    std::function<void(shared_ptr<Item>)> dropFunc;
    inline string sign(int x);
    QMenu* wearableWeaponSubMenu;
    QMenu* itemMenu;
    QAction* use;
    QAction* asWeapon;
    QAction* asWearable;
    QAction* drop;
    QAction* close;
    void setItemInteraction(shared_ptr<Item> item);
    void invListUpdated();
    void equListUpdated();
    public:
    QPushButton* changeInvButton;
    QLabel* playerStats;
    QLabel* inventoryType;
    QListWidget* rightInventoryList;
    QListWidget* rightEquipmentList;
    InventoryWidget(Player& player, std::function<void(shared_ptr<Item>)> dropFunc);
    ~InventoryWidget();
    void updateStats();
    void updateEquipment(int type);
    void updateInventory(int index);
    void changeDisplay();
    void unEquip(int type);
    void equip(shared_ptr<Item> itemToEquip, int slot);
};

#endif // __INVENTORYWIDGET_H__
