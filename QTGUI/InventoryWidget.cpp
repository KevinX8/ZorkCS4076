
#include "InventoryWidget.h"
InventoryWidget::InventoryWidget(Player* player, GameInstance instance)
{
    this->player = player;
    this->instance = instance;
    updateStats();
    playerItems = playerItems(3);
    itemMenu = QMenu(this);
    wearableWeaponSubMenu = QMenu("Equip As: ",this);
    QAction asWeapon = QAction("Weapon",wearableWeaponSubMenu);
    QAction asWearable = QAction("Wearable",wearableWeaponSubMenu);
    wearableWeaponSubMenu.addAction(asWeapon);
    wearableWeaponSubMenu.addAction(asWearable);
    use = QAction("Equip",itemMenu);
    QAction close = QAction("Close",this);
    QAction drop = QAction("Drop",this);
    itemMenu.addAction(drop);
    itemMenu.addAction(close);
    connect(close, &QAction::triggered(),this,&itemMenu.hide());
    rightInventoryView.addActions(playerItems);
    rightEquipmentView.addActions(equipedItems);
}

void InventoryWidget::updateStats() 
{
    playerStats.setText("⚔️ " + player->getStrength() + " 🗣️ " + player->getCharisma() + " ☘️ " + player->getLuck() + " 🎒 " + player->inventory.size() + "/" + player->inventorySpace());
}

void InventoryWidget::updateEquipment(int type) 
{
    int itemCode = 0;
    switch (type) {
        case 0: equipedItems[type] = QAction(player->activeWeapon->description()); itemCode = player->activeWeapon->hashCode; break;
        case 1: equipedItems[type] = QAction(player->activeWearable1->description()); itemCode = player->activeWearable1->hashCode; break;
        case 2: equipedItems[type] = QAction(player->activeWearable2->description()); itemCode = player->activeWearable2->hashCode; break;
    }
    QAction close = itemMenu.actionAt(1);
    QAction unEquipAct = QAction("Unequip",this);
    connect(unEquipAct,&QAction::triggered(),itemMenu,&unEquip(type));
    itemMenu.clear();
    itemMenu.addAction(unEquipAct);
    itemMenu.addAction(close);
    connect(equipedItems[type],&QAction::triggered(),this,itemMenu.popup(this));
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

void InventoryWidget::unEquip(int type) 
//Don't shift equipment up to avoid shifting all data structures related to it
{
    player->unequip(type);
    updateEquipment[type] = QAction("");
}

void InventoryWidget::equip(shared_ptr<Item> itemToEquip, int slot) 
{
    
}

void InventoryWidget::setItemInteraction(shared_ptr<Item> item) 
{
    QAction drop = itemMenu.actionAt(0);
    QAction close = itemMenu.actionAt(1);
    itemMenu.clear();
    if (itemCode >= TOTAL_ITEMS - NUM_WEARABLEWEAPONS) {
        itemMenu.addMenu(wearableWeaponSubMenu);
        connect(wearableWeaponSubMenu.actionAt(0),&QAction::triggered(),wearableWeaponSubMenu,&equip(itemToEquip,0));
        connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&equip(itemToEquip,1));
        if (!player->activeWearable1) {
            connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&player->equip(itemCode,1));
        } else if (!player->activeWearable2) {
            connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&player->equip(itemCode,2));
        } else {
            connect(wearableWeaponSubMenu.actionAt(1),&QAction::triggered(),wearableWeaponSubMenu,&wearableWeaponSubMenu.actionAt(1)->setText("Unequip a wearble first!"));
        }
    } else if (itemCode >= NUM_STD_ITEMS) {
        itemMenu.addAction(use);
        connect(use,&QAction::triggered(),rightInventoryView, &equip(item));
    }
    itemMenu.addAction(drop);
    connect(drop,&QAction::triggered(),rightInventoryView,&instance.interactDropPlayerInv(item));
    ddAction(close);
}
