 
#include "RoomItemWidget.h"

RoomItemWidget::RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc)
{
    this->roomItemFunc = roomItemFunc;
    itemMenu = unique_ptr<QMenu>(new QMenu(this));
    pickup = unique_ptr<QAction>(new QAction("Pickup",this));
    close = unique_ptr<QAction>(new QAction("Close",this));
    roomItemsLabel = unique_ptr<QLabel>(new QLabel(QString::fromStdString("Items in Room: " + to_string(roomItems.size())),this));
    listView = unique_ptr<QListView>(new QListView(this));
    itemMenu->addAction(pickup.get());
    itemMenu->addAction(close.get());
    connect(close.get(), &QAction::triggered,this,[this](){itemMenu->hide();});
    this->updateItems(itemCodes);
    listView->addActions(roomItems);
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    roomItems.clear();
    int index = 0;
    for (int itemCode: itemCodes) {
        unique_ptr<QAction> itemAction = unique_ptr<QAction>(new QAction(QString::fromStdString(Item::getType(itemCode) + Item::itemNameMap.at(itemCode)),this));
        connect(itemAction.get(), &QAction::triggered, this, [this,index](){setItemInteraction(index);});
        //roomItems.push_back(itemAction.get());
        listView->addAction(itemAction.get());
        index++;
    }
    roomItemsLabel->setText(QString::fromStdString("Items in Room: " + to_string(index+1)));
    //listView->addActions(roomItems);
}

void RoomItemWidget::setItemInteraction(int index) 
{
    connect(pickup.get(), &QAction::triggered,this,[this,index](){roomItemFunc(index);});
    itemMenu->popup(QCursor::pos());
}
