#include "InventoryWidget.h"
#include <string>

InventoryWidget::InventoryWidget(Player& player, std::function<void(shared_ptr<Item>)> dropFunc) : player(player)
{
    this->player = player;
    this->dropFunc = dropFunc;
    playerStats = new QLabel(this);
    updateStats();
    itemMenu = new QMenu(this);
    wearableWeaponSubMenu = new QMenu("Equip As: ",this);
    asWeapon = new QAction("Weapon",this);
    asWearable = new QAction("Wearable",this);
    wearableWeaponSubMenu->addAction(asWeapon);
    wearableWeaponSubMenu->addAction(asWearable);
    use = new QAction("Equip",this);
    close = new QAction("Close",this);
    drop = new QAction("Drop",this);
    changeInvButton = new QPushButton("🎒",this);
    rightInventoryList = new QListWidget(this);
    rightEquipmentList = new QListWidget(this);
    itemMenu->addAction(drop);
    itemMenu->addAction(close);
    connect(changeInvButton, &QPushButton::released,this,[this](){changeDisplay();});
    connect(close, &QAction::triggered,this,[this](){itemMenu->hide();});
    inventoryType = new QLabel("Inventory 🧰",this);
    rightInventoryList->show();
    rightEquipmentList->hide();
    connect(rightInventoryList, &QListWidget::itemClicked,this,&InventoryWidget::invListUpdated);
    connect(rightEquipmentList,&QListWidget::itemClicked,this,&InventoryWidget::equListUpdated);
}

void InventoryWidget::updateStats() 
{
    QString stats = QString::fromStdString("⚔️ " + std::to_string(player.getStrength()) + " 🗣️ " + std::to_string(player.getCharisma()) + " ☘️ " + std::to_string(player.getLuck()) + " 🎒 " + std::to_string(player.inventory.size()) + "/" + std::to_string(player.inventorySpace));
    playerStats->setText(stats);
    playerStats->resize(200,30);
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
   /* delete(rightEquipmentList); Qt objects do not need to be manually deleted due to parent memory management
    delete(rightInventoryList);
    delete(wearableWeaponSubMenu);
    delete(itemMenu);
    delete(use);
    delete(asWeapon);
    delete(asWearable);
    delete(drop);
    delete(close);
    delete(changeInvButton);
    delete(playerStats);
    delete(inventoryType); */
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
    switch (type) {
        case 0: rightInventoryList->addItem(QString::fromStdString(player.activeWeapon->getShortDescription())); break;
        case 1: rightInventoryList->addItem(QString::fromStdString(player.activeWearable1->getShortDescription())); break;
        case 2: rightInventoryList->addItem(QString::fromStdString(player.activeWearable2->getShortDescription())); break;
    }
    player.unequip(type);
    delete(rightEquipmentList->takeItem(type));
    updateStats();
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
            itemMenu->addAction(drop);
            itemMenu->addAction(close);
        }
    }
    updateStats();
    updateInventory(player.inventory.size()-1);
}

void InventoryWidget::setItemInteraction(shared_ptr<Item> item) 
{
    itemMenu->clear();
    use->disconnect();
    drop->disconnect();
    if (item->hashCode >= TOTAL_ITEMS - NUM_WEARABLEWEAPONS) {
        asWeapon->disconnect();
        asWearable->disconnect();
        itemMenu->addMenu(wearableWeaponSubMenu);
        connect(asWeapon,&QAction::triggered,this,[this,item](){ equip(item,0);});
        connect(asWearable,&QAction::triggered,this,[this,item](){ equip(item,1);});
    } else if (item->hashCode >= NUM_STD_ITEMS) {
        itemMenu->addAction(use);
        if (item->hashCode < NUM_STD_ITEMS+NUM_WEAPONS) {
            connect(use,&QAction::triggered,this,[this,item](){ equip(item,0);});
        } else {
            connect(use,&QAction::triggered,this,[this,item](){ equip(item,1);});
        }
    }
    itemMenu->addAction(drop);
    connect(drop,&QAction::triggered,this,[this,item](){InventoryWidget::dropFunc(item);});
    itemMenu->addAction(close);
}

void InventoryWidget::invListUpdated() 
{
    for (int i=0; i < rightInventoryList->count();i++) {
        if (rightInventoryList->item(i)->isSelected()) {
            if (!giving) {
                setItemInteraction(player.inventory.at(i));
            } else {
                setGiveItem(i);
            }
            itemMenu->popup(QCursor::pos());
        }
    }
}

void InventoryWidget::equListUpdated() 
{
    for (int i=0; i < rightEquipmentList->count();i++) {
        if (rightEquipmentList->item(i)->isSelected()) {
                auto unEquipAct = new QAction("Unequip",this);
                connect(unEquipAct,&QAction::triggered,itemMenu,[this,i](){this->unEquip(i);});
                itemMenu->clear();
                itemMenu->addAction(unEquipAct);
                itemMenu->addAction(close);
                itemMenu->popup(QCursor::pos());
        }
    }
}

void QTGui::InventoryWidget::setGiveItem(int index) 
{
    itemMenu->clear();
    QAction* give = new QAction("Give NPC",this);
    shared_ptr<Item> item = player.inventory.at(index);
    connect(give,&QAction::triggered,this,[this,item]{InventoryWidget::dropFunc(item);});
    itemMenu->addAction(give);
    itemMenu->addAction(close);
}
