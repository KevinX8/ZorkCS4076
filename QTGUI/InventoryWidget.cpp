
#include "InventoryWidget.h"
InventoryWidget::InventoryWidget(Player& player, std::function<void(shared_ptr<Item>)> dropFunc) : player(player)
{
    this->player = player;
    this->dropFunc = dropFunc;
    updateStats();
    playerItems.reserve(3);
    itemMenu = unique_ptr<QMenu>(new QMenu(this));
    wearableWeaponSubMenu = unique_ptr<QMenu>(new QMenu("Equip As: ",this));
    asWeapon = unique_ptr<QAction>(new QAction("Weapon",wearableWeaponSubMenu.get()));
    asWearable = unique_ptr<QAction>(new QAction("Wearable",wearableWeaponSubMenu.get()));
    wearableWeaponSubMenu->addAction(asWeapon.get());
    wearableWeaponSubMenu->addAction(asWearable.get());
    use = unique_ptr<QAction>(new QAction("Equip",itemMenu.get()));
    close = unique_ptr<QAction>(new QAction("Close",this));
    drop = unique_ptr<QAction>(new QAction("Drop",this));
    changeInvButton = unique_ptr<QPushButton>(new QPushButton("🎒",this));
    itemMenu->addAction(drop.get());
    itemMenu->addAction(close.get());
    connect(changeInvButton.get(), &QPushButton::released,this,[this](){changeDisplay();});
    connect(close.get(), &QAction::triggered,this,[this](){itemMenu->hide();});
    rightInventoryView.addActions(playerItems);
    rightEquipmentView.addActions(equipedItems);
    inventoryType = unique_ptr<QLabel>(new QLabel("Inventory 🧰",this));
    rightInventoryView.show();
    rightEquipmentView.hide();
}

void InventoryWidget::updateStats() 
{
    QString stats = QString::fromStdString("⚔️ " + std::to_string(player.getStrength()) + " 🗣️ " + std::to_string(player.getCharisma()) + " ☘️ " + std::to_string(player.getLuck()) + " 🎒 " + std::to_string(player.inventory.size()) + "/" + std::to_string(player.inventorySpace));
    playerStats->setText(stats);
}

void InventoryWidget::updateEquipment(int type) 
{
    int itemCode = 0;
    switch (type) {
        case 0: equipedItems[type] = new QAction(QString::fromStdString(player.activeWeapon->getShortDescription())); itemCode = player.activeWeapon->hashCode; break;
        case 1: equipedItems[type] = new QAction(QString::fromStdString(player.activeWearable1->getShortDescription())); itemCode = player.activeWearable1->hashCode; break;
        case 2: equipedItems[type] = new QAction(QString::fromStdString(player.activeWearable2->getShortDescription())); itemCode = player.activeWearable2->hashCode; break;
    }
    unique_ptr<QAction> unEquipAct = unique_ptr<QAction>(new QAction("Unequip",this));
    connect(unEquipAct.get(),&QAction::triggered,itemMenu.get(),[this,type](){unEquip(type);});
    itemMenu->clear();
    itemMenu->addAction(unEquipAct.get());
    itemMenu->addAction(close.get());
    connect(equipedItems[type],&QAction::triggered,this,[this](){itemMenu->popup(QCursor::pos());});
}

void InventoryWidget::updateInventory(int index) 
{
    if (index < playerItems.size()) {
    playerItems.erase(playerItems.begin() + index);
    } else {
        unique_ptr<QAction> item = unique_ptr<QAction>(new QAction(QString::fromStdString(player.inventory.at(index)->getShortDescription()),this));
        connect(item.get(), &QAction::triggered,this,[this,index](){setItemInteraction(player.inventory.at(index));});
        playerItems.push_back(item.get());
    }
}

void InventoryWidget::changeDisplay()
{
    if (rightInventoryView.isHidden()) {
        inventoryType->setText("Inventory 🧰");
        rightInventoryView.show();
        rightEquipmentView.hide();
    } else {
        inventoryType->setText("Equipment 🛠️");
        rightInventoryView.hide();
        rightEquipmentView.show();
    }
}

void InventoryWidget::unEquip(int type) 
//Don't shift equipment up to avoid shifting all data structures related to it
{
    player.unequip(type);
    equipedItems[type] = new QAction("");
}

void InventoryWidget::equip(shared_ptr<Item> itemToEquip, int slot) 
{
    if (slot == 0) {
        player.equip(itemToEquip,0);
    } else if (slot > 0) {
    if (!player.activeWearable1) {
        player.equip(itemToEquip,1);
    } else if (!player.activeWearable2) {
        player.equip(itemToEquip,2);
    } else {
        itemMenu->clear();
        itemMenu->addAction(new QAction("Unequip a wearble first!"));
        itemMenu->addAction(drop.get());
        itemMenu->addAction(close.get());
    }
    }
}

void InventoryWidget::setItemInteraction(shared_ptr<Item> item) 
{
    itemMenu->clear();
    if (item->hashCode >= TOTAL_ITEMS - NUM_WEARABLEWEAPONS) {
        itemMenu->addMenu(wearableWeaponSubMenu.get());
        connect(asWeapon.get(),&QAction::triggered,this,[this,item](){ equip(item,0);});
        connect(asWearable.get(),&QAction::triggered,this,[this,item](){ equip(item,1);});
    } else if (item->hashCode >= NUM_STD_ITEMS && item->hashCode < NUM_WEAPONS) {
        itemMenu->addAction(use.get());
        connect(use.get(),&QAction::triggered,this,[this,item](){ equip(item,0);});
    }
    itemMenu->addAction(drop.get());
    connect(drop.get(),&QAction::triggered,this,[this,item](){InventoryWidget::dropFunc(item);});
    itemMenu->addAction(drop.get());
}
