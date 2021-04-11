
#include "InventoryWidget.h"
#include <string>

InventoryWidget::InventoryWidget(Player& player, std::function<void(shared_ptr<Item>)> dropFunc) : player(player)
{
    this->player = player;
    this->dropFunc = dropFunc;
    playerStats = new QLabel(this);
    updateStats();
    itemMenu = new QMenu(this);
    //setup wearable weapon
    wearableWeaponSubMenu = new QMenu("Equip As: ",this);
    asWeapon = new QAction("Weapon",this);
    asWearable = new QAction("Wearable",this);
    wearableWeaponSubMenu->addAction(asWeapon);
    wearableWeaponSubMenu->addAction(asWearable);
    //setup general equipment buttons
    use = new QAction("Equip",this);
    close = new QAction("Close",this);
    drop = new QAction("Drop",this);
    changeInvButton = new QPushButton("🎒",this);
    rightInventoryList = new QListWidget(this);
    rightEquipmentList = new QListWidget(this);
    //setup empty slots to avoid crashing
    rightEquipmentList->addItem("Empty Weapon Slot");
    rightEquipmentList->addItem("Empty Wearable Slot 1");
    rightEquipmentList->addItem("Empty Wearable Slot 2");
    itemMenu->addAction(drop);
    itemMenu->addAction(close);
    //connect inventory/equipment switch button
    connect(changeInvButton, &QPushButton::released,this,[this](){changeDisplay();});
    connect(close, &QAction::triggered,this,[this](){itemMenu->hide();});
    inventoryType = new QLabel("Inventory 🧰",this);
    //show inventory by default
    rightInventoryList->show();
    rightEquipmentList->hide();
    //connect relevant list to list update function
    connect(rightInventoryList, &QListWidget::itemClicked,this,&InventoryWidget::invListUpdated);
    connect(rightEquipmentList,&QListWidget::itemClicked,this,&InventoryWidget::equListUpdated);
}

void InventoryWidget::updateStats() 
{
    //reads players stats and redisplays them
    QString stats = QString::fromStdString("⚔️ " + std::to_string(player.getStrength()) + " 🗣️ " + std::to_string(player.getCharisma()) + " ☘️ " + std::to_string(player.getLuck()) + " 🎒 " + std::to_string(player.inventory.size()) + "/" + std::to_string(player.inventorySpace));
    playerStats->setText(stats);
    playerStats->resize(200,30);
}

inline string InventoryWidget::sign(int x) {
    return ((x > 0) - (x < 0)) == -1? "-":"+";
}

void InventoryWidget::updateEquipment(int type)
{
    //remove equipment item that is current there as it will be replaced
    delete(rightEquipmentList->takeItem(type));
    QString name;
    //generate the name prettily with buffs and debuffs displayed etc.
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
    //remove item from inventory list widget if the item that changed was below inventory size
    if (index < rightInventoryList->count()) {
        delete(rightInventoryList->takeItem(index));
    } else { //add item otherwise
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
    //swap display from inventory to equipment and vice versa
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
//adds item to inventory list widget and replaces the item in the equipment list widget with the empty type
{
    switch (type) {
        case 0: rightInventoryList->addItem(QString::fromStdString(player.activeWeapon->getShortDescription())); rightEquipmentList->insertItem(1,"Empty Weapon Slot"); break;
        case 1: rightInventoryList->addItem(QString::fromStdString(player.activeWearable1->getShortDescription())); rightEquipmentList->insertItem(2,"Empty Wearable Slot 1"); break;
        case 2: rightInventoryList->addItem(QString::fromStdString(player.activeWearable2->getShortDescription())); rightEquipmentList->addItem("Empty Wearable Slot 2"); break;
    }
    //removes it from the player objects inventory next
    player.unequip(type);
    delete(rightEquipmentList->takeItem(type));
    updateStats();
}

void InventoryWidget::equip(shared_ptr<Item> itemToEquip, int slot) 
{
    //checks which slot the item is supposed to equip too
    if (slot == 0) {
        player.equip(itemToEquip,0);
        updateEquipment(slot);
        updateInventory(player.takeItem(itemToEquip->hashCode));
    } else if (slot > 0) {
        if (!player.activeWearable1) { //if slot 1 is full for wearables attempt to equip it to slot 2 instead
            player.equip(itemToEquip,1);
            updateEquipment(1);
            updateInventory(player.takeItem(itemToEquip->hashCode));
        } else if (!player.activeWearable2) {
            player.equip(itemToEquip,2);
            updateEquipment(2);
            updateInventory(player.takeItem(itemToEquip->hashCode));
        } else { //if both slots are full tell the player to unequip first as we are not going to guess which slot they want to use for them
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
{ //this checks what item it is from the list and displays the appropriate drop down menu items
    itemMenu->clear();
    use->disconnect();
    drop->disconnect();
    if (item->hashCode >= TOTAL_ITEMS - NUM_WEARABLEWEAPONS) { //wearable weapons have a unqiue drop down menu
        asWeapon->disconnect();
        asWearable->disconnect();
        itemMenu->addMenu(wearableWeaponSubMenu);
        connect(asWeapon,&QAction::triggered,this,[this,item](){ equip(item,0);});
        connect(asWearable,&QAction::triggered,this,[this,item](){ equip(item,1);});
    } else if (item->hashCode >= NUM_STD_ITEMS) { //weapons and wearables have the "use" option
        itemMenu->addAction(use);
        if (item->hashCode < NUM_STD_ITEMS+NUM_WEAPONS) {
            connect(use,&QAction::triggered,this,[this,item](){ equip(item,0);});
        } else {
            connect(use,&QAction::triggered,this,[this,item](){ equip(item,1);});
        }
    } // standard items only have drop and close like all other items
    itemMenu->addAction(drop);
    connect(drop,&QAction::triggered,this,[this,item](){InventoryWidget::dropFunc(item);});
    itemMenu->addAction(close);
}

void InventoryWidget::invListUpdated() 
{ //this finds the index of the item that was just pressed (Qt thangs)
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
{ //this finds the index of the item that was just pressed, the same as inventory but also checks first if the slot is empty or not
    for (int i=0; i < rightEquipmentList->count();i++) {
        if (rightEquipmentList->item(i)->isSelected()) {
            bool valid = false;
            switch (i) {
                case 0: valid = !!player.activeWeapon; break;
                case 1: valid = !!player.activeWearable1; break;
                case 2: valid = !!player.activeWearable2; break;
            }
                itemMenu->clear();
                if (valid) {
                auto unEquipAct = new QAction("Unequip",this);
                connect(unEquipAct,&QAction::triggered,itemMenu,[this,i](){this->unEquip(i);});
                itemMenu->addAction(unEquipAct);
                }
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
