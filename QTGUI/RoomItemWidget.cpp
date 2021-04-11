 #include "RoomItemWidget.h"
#include <QDebug>

RoomItemWidget::RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc)
{
    //set function required for room <-> player interaction
    this->roomItemFunc = roomItemFunc;
    //sets up menu with relevant actions
    itemMenu = new QMenu(this);
    pickup = new QAction("Pickup",this);
    close = new QAction("Close",this);
    roomItemsLabel = new QLabel(QString::fromStdString("Items in Room: 0"),this);
    //roomItemsLabel->move(510-(roomItemsLabel->width()+50),0);
    listWidget = new QListWidget(this);
    itemMenu->addAction(pickup);
    itemMenu->addAction(close);
    connect(close, &QAction::triggered,this,[this](){itemMenu->hide();});
    connect(listWidget,&QListWidget::itemClicked,this,&RoomItemWidget::listUpdated);
    this->updateItems(itemCodes);
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    //reads the vector of itemcodes inside a room and creates the list widget based off them
    int index = 0;
    listWidget->clear();
    for (int itemCode: itemCodes) {
        //displays the items type and name
        QListWidgetItem* temp = new QListWidgetItem(QString::fromStdString(Item::getType(itemCode) + Item::itemNameMap.at(itemCode)));
        listWidget->addItem(temp);
        index++;
    }
    //updates the label that shows the amount of items in a room
    roomItemsLabel->setText(QString::fromStdString("Items in Room: " + to_string(index)));
}

void RoomItemWidget::setItemInteraction(int index) 
{
    //sets the function to be executed when the player chooses the pickup option based on the item index in the room inventory
    connect(pickup, &QAction::triggered,this,[this,index](){roomItemFunc(index);disconnect(pickup);});
    itemMenu->popup(QCursor::pos());
}

void RoomItemWidget::listUpdated()
{ //finds the index of the item in the list widget
    pickup->disconnect();
    for (int i=0;i < listWidget->count();i++) {
        if (listWidget->item(i)->isSelected()) {
            setItemInteraction(i);
        }
    }
}

QTGui::RoomItemWidget::~RoomItemWidget() 
{
 /* delete(listWidget); Qt objects do not need to be manually deleted due to parent memory management
    delete(roomItemsLabel);
    delete(itemMenu);
    delete(pickup);
    delete(close); */
}
