
#include "InventoryWidget.h"
InventoryWidget::InventoryWidget(Player* player) 
{
    this->player = player;
    updateStats();
    itemMenu = QMenu(this);
    wearableWeaponSubMenu = QMenu("Equip As: ",this);
    QAction asWeapon = QAction("Weapon",wearableWeaponSubMenu);
    QAction asWearable = QAction("Wearable",wearableWeaponSubMenu);
    wearableWeaponSubMenu.addAction(asWeapon);
    wearableWeaponSubMenu.addAction(asWearable);
    use = QAction("Use",itemMenu);
    QAction close = QAction("Close",this);
    itemMenu.addAction(use);
    itemMenu.addAction(close);
    connect(close, &QAction::triggered(),this,&itemMenu.hide());
    rightInventoryView.addActions(playerItems);
    rightEquipmentView.addActions(equipedItems);
}

void InventoryWidget::updateStats() 
{
    playerStats.setText("⚔️ " + player->getStrength() + "🗣️ " + player->getCharisma() + "☘️ " + player->getLuck());
}

void InventoryWidget::updateEquipment(int type) 
{
    int itemCode = 0;
    switch (type) {
        case 0: equipedItems[type] = QAction(player->activeWeapon->description()); itemCode = player->activeWeapon->hashCode; break;
        case 1: equipedItems[type] = QAction(player->activeWearable1->description()); itemCode = player->activeWearable1->hashCode; break;
        case 2: equipedItems[type] = QAction(player->activeWearable2->description()); itemCode = player->activeWearable2->hashCode; break;
    }
    connect(equipedItems[type], &QAction::triggered(),this,&setItemInteraction(itemCode));
}

void InventoryWidget::updateInventory(int index) 
{
    if (index < playerItems.size()) {
    playerItems.erase(playerItems.begin() + index);
    } else {
    QAction item = QAction(player->inventory.at(index).getShortDescription(),playerItems);
    connect(item, &QAction::triggered(),rightInventoryView,&setItemInteraction(itemCode));
    playerItems.add(item);
    }
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

void InventoryWidget::setItemInteraction(int itemCode) 
{
    QAction close = itemMenu.actionAt(1);
    itemMenu.clear();
    if (itemCode >= TOTAL_ITEMS - NUM_WEARABLEWEAPONS) {
        itemMenu.addMenu(wearableWeaponSubMenu);
        connect(wearableWeaponSubMenu.actionAt(0),&QAction::triggered(),wearableWeaponSubMenu,&player->equip(itemCode,0));
        if (!player->activeWearable1) {
            connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&player->equip(itemCode,1));
        } else if (!player->activeWearable2) {
            connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&player->equip(itemCode,2));
        } else {
            connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&wearableWeaponSubMenu.actionAt(1)->setText("Unequip a wearble first!"));
        }
        itemMenu.addAction(close);
    } else {
        itemMenu.addAction(use);
        itemMenu.addAction(close);
    }
}
