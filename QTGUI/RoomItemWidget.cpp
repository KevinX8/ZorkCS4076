 
#include "RoomItemWidget.h"

RoomItemWidget::RoomItemWidget(vector<int> itemCodes, GameInstance &instance) : listView(Qwidget* MainWindow), instance(instance)
{
    itemMenu(this);
    QAction pickup = QAction("Pickup",this);
    QAction close = QAction("Close",this);
    itemMenu.addAction(pickup);
    itemMenu.addAction(close);
    connect(close, &QAction::triggered(),this,&itemMenu.hide());
    this->updateItems(itemCodes, instance);
    listView.addActions(roomItems);
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    roomItems.clear();
    for (int itemCode: itemIndexes) {
        QAction itemAction = QAction(Item::getType(itemCode) + Item::itemNameMap.at(itemCode),this);
        connect(itemAction, &QAction::triggered(), this, &setItemInteraction(itemCode));
        roomItems.push_back(itemAction);
    }
}

void RoomItemWidget::setItemInteraction(int itemCode) 
{
    connect(itemMenu.actionAt(0), &QAction::triggered(),itemMenu,&instance.interactRoomItem(itemCode);
    itemMenu.popup(this);
}
