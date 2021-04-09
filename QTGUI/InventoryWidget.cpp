#include "InventoryWidget.h"
#include <string>

InventoryWidget::InventoryWidget(Player& player, std::function<void(shared_ptr<Item>)> dropFunc) : player(player)
{
    this->player = player;
    this->dropFunc = dropFunc;
    playerStats = unique_ptr<QLabel>(new QLabel(this));
    updateStats();
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
    rightInventoryList = new QListWidget(this);
    rightEquipmentList = new QListWidget(this);
    itemMenu->addAction(drop.get());
    itemMenu->addAction(close.get());
    connect(changeInvButton.get(), &QPushButton::released,this,[this](){changeDisplay();});
    connect(close.get(), &QAction::triggered,this,[this](){itemMenu->hide();});
    inventoryType = unique_ptr<QLabel>(new QLabel("Inventory 🧰",this));
    rightInventoryList->show();
    rightEquipmentList->hide();
    connect(rightInventoryList, &QListWidget::itemClicked,this,&InventoryWidget::invListUpdated);
    connect(rightEquipmentList,&QListWidget::itemClicked,this,&InventoryWidget::equListUpdated);
}

void InventoryWidget::updateStats() 
{
    QString stats = QString::fromStdString("⚔️ " + std::to_string(player.getStrength()) + " 🗣️ " + std::to_string(player.getCharisma()) + " ☘️ " + std::to_string(player.getLuck()) + " 🎒 " + std::to_string(player.inventory.size()) + "/" + std::to_string(player.inventorySpace));
    playerStats->setText(stats);
}

inline string InventoryWidget::sign(int x) {
    return ((x > 0) - (x < 0)) == -1? "-":"+";
}

void InventoryWidget::updateEquipment(int type)
{
    delete(rightEquipmentList->takeItem(type));
    QString name;
    switch (type) {
        case 0: name = QString::fromStdString(player.activeWeapon->getShortDescription() + " ⚔️" + sign(player.activeWeapon->getDamage()) + to_string(player.activeWeapon->getDamage()) STRINGEND(activeWeapon)); break;
        case 1: name = WEARABLESTRING(activeWearable1); break;
        case 2: name = WEARABLESTRING(activeWearable2); break;
    }
    rightEquipmentList->insertItem(type,new QListWidgetItem(name));
    updateStats();
}

void InventoryWidget::updateInventory(int index)
{
    if (index < rightInventoryList->count()) {
        delete(rightInventoryList->takeItem(index));
    } else {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(player.inventory.at(index)->getShortDescription()));
        rightInventoryList->addItem(item);
    }
}

QTGui::InventoryWidget::~InventoryWidget() 
{
    delete(rightEquipmentList);
    delete(rightInventoryList);
}

void InventoryWidget::changeDisplay()
{
    if (rightInventoryList->isHidden()) {
        inventoryType->setText("Inventory 🧰");
        rightInventoryList->show();
        rightEquipmentList->hide();
    } else {
        inventoryType->setText("Equipment 🛠️");
        rightInventoryList->hide();
        rightEquipmentList->show();
    }
}

void InventoryWidget::unEquip(int type) 
//Don't shift equipment up to avoid shifting all data structures related to it
{
    player.unequip(type);
    switch (type) {
        case 0: rightInventoryList->addItem(QString::fromStdString(player.activeWeapon->getShortDescription())); break;
        case 1: rightInventoryList->addItem(QString::fromStdString(player.activeWearable1->getShortDescription())); break;
        case 2: rightInventoryList->addItem(QString::fromStdString(player.activeWearable2->getShortDescription())); break;
    }
    delete(rightEquipmentList->takeItem(type));
}

void InventoryWidget::equip(shared_ptr<Item> itemToEquip, int slot) 
{
    if (slot == 0) {
        player.equip(itemToEquip,0);
        updateEquipment(slot);
        updateInventory(player.takeItem(itemToEquip->hashCode));
    } else if (slot > 0) {
        if (!player.activeWearable1) {
            player.equip(itemToEquip,1);
            updateEquipment(1);
            updateInventory(player.takeItem(itemToEquip->hashCode));
        } else if (!player.activeWearable2) {
            player.equip(itemToEquip,2);
            updateEquipment(2);
            updateInventory(player.takeItem(itemToEquip->hashCode));
        } else {
            itemMenu->clear();
            itemMenu->addAction(new QAction("Unequip a wearble first!"));
            itemMenu->addAction(drop.get());
            itemMenu->addAction(close.get());
        }
    }
    updateStats();
}

void InventoryWidget::setItemInteraction(shared_ptr<Item> item) 
{
    itemMenu->clear();
    if (item->hashCode >= TOTAL_ITEMS - NUM_WEARABLEWEAPONS) {
        itemMenu->addMenu(wearableWeaponSubMenu.get());
        connect(asWeapon.get(),&QAction::triggered,this,[this,item](){ equip(item,0);});
        connect(asWearable.get(),&QAction::triggered,this,[this,item](){ equip(item,1);});
    } else if (item->hashCode >= NUM_STD_ITEMS) {
        itemMenu->addAction(use.get());
        if (item->hashCode < NUM_STD_ITEMS+NUM_WEAPONS) {
            connect(use.get(),&QAction::triggered,this,[this,item](){ equip(item,0);});
        } else {
            connect(use.get(),&QAction::triggered,this,[this,item](){ equip(item,1);});
        }
    }
    itemMenu->addAction(drop.get());
    connect(drop.get(),&QAction::triggered,this,[this,item](){InventoryWidget::dropFunc(item);});
    itemMenu->addAction(close.get());
}

void InventoryWidget::invListUpdated() 
{
    for (int i=0; i < rightInventoryList->count();i++) {
        if (rightInventoryList->item(i)->isSelected()) {
            setItemInteraction(player.inventory.at(i));
            itemMenu->popup(QCursor::pos());
        }
    }
}

void InventoryWidget::equListUpdated() 
{
    for (int i=0; i < rightEquipmentList->count();i++) {
        if (rightEquipmentList->item(i)->isSelected()) {
                QAction* unEquipAct = new QAction("Unequip",this);
                connect(unEquipAct,&QAction::triggered,itemMenu.get(),[this,i](){this->unEquip(i);});
                itemMenu->clear();
                itemMenu->addAction(unEquipAct);
                itemMenu->addAction(close.get());
                itemMenu->popup(QCursor::pos());
        }
    }
}
