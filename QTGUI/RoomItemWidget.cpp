 
#include "RoomItemWidget.h"

RoomItemWidget::RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc)
{
    this->roomItemFunc = roomItemFunc;
    itemMenu = unique_ptr<QMenu>(new QMenu(this));
    pickup = unique_ptr<QAction>(new QAction("Pickup",this));
    close = unique_ptr<QAction>(new QAction("Close",this));
    itemMenu->addAction(pickup.get());
    itemMenu->addAction(close.get());
    connect(close.get(), &QAction::triggered,this,[this](){itemMenu->hide();});
    this->updateItems(itemCodes);
    listView.addActions(roomItems);
    roomItemsLabel = unique_ptr<QLabel>(new QLabel(QString::fromStdString("Items in Room: " + to_string(roomItems.size())),this));
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    roomItems.clear();
    for (int itemCode: itemCodes) {
        unique_ptr<QAction> itemAction = unique_ptr<QAction>(new QAction(QString::fromStdString(Item::getType(itemCode) + Item::itemNameMap.at(itemCode)),this));
        connect(itemAction.get(), &QAction::triggered, this, [this,itemCode](){setItemInteraction(itemCode);});
        roomItems.push_back(itemAction.get());
    }
    roomItemsLabel->setText(QString::fromStdString("Items in Room: " + to_string(roomItems.size())));
}

void RoomItemWidget::setItemInteraction(int itemCode) 
{
    connect(pickup.get(), &QAction::triggered,this,[this,itemCode](){roomItemFunc(itemCode);});
    itemMenu->popup(QCursor::pos());
}
