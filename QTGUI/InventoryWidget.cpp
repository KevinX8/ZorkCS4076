
#include "InventoryWidget.h"
InventoryWidget::InventoryWidget(Player* player) 
{
    this->player = player;

}

void InventoryWidget::updateStats() 
{
    playerStats.setText("⚔️ " + player->getStrength() + "🗣️ " + player->getCharisma() + "☘️ " + player->getLuck());
}

void InventoryWidget::updateEquipment(int type) 
{
    switch (type) {
        case 0: equipedItems[type] = QAction(player->activeWeapon->description());
        case 1: equipedItems[type] = QAction(player->activeWearable1->description());
        case 2: equipedItems[type] = QAction(player->activeWearable2->description());
    }
}

void InventoryWidget::updateInventory(int index) 
{
    playerItems.erase(index);
    playerItems.add(player->inventory.at(index));
}

void InventoryWidget::changeDisplay(bool showInventory) 
{
    if (showInventory) {
        rightInventoryView.show();
        rightEquipmentView.hide();
    } else {
        rightInventoryView.hide();
        rightEquipmentView.show();
    }
}
