 #include "RoomItemWidget.h"
#include <QDebug>

RoomItemWidget::RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc)
{
    this->roomItemFunc = roomItemFunc;
    itemMenu = unique_ptr<QMenu>(new QMenu(this));
    pickup = unique_ptr<QAction>(new QAction("Pickup",this));
    close = unique_ptr<QAction>(new QAction("Close",this));
    roomItemsLabel = unique_ptr<QLabel>(new QLabel(QString::fromStdString("Items in Room: 0"),this));
    //roomItemsLabel->move(510-(roomItemsLabel->width()+50),0);
    listWidget = new QListWidget(this);
    itemMenu->addAction(pickup.get());
    itemMenu->addAction(close.get());
    connect(close.get(), &QAction::triggered,this,[this](){itemMenu->hide();});
    connect(listWidget,&QListWidget::itemClicked,this,&RoomItemWidget::listUpdated);
    this->updateItems(itemCodes);
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    int index = 0;
    listWidget->clear();
    for (int itemCode: itemCodes) {
        QListWidgetItem* temp = new QListWidgetItem(QString::fromStdString(Item::getType(itemCode) + Item::itemNameMap.at(itemCode)));
        listWidget->addItem(temp);
        index++;
    }
    roomItemsLabel->setText(QString::fromStdString("Items in Room: " + to_string(index)));
}

void RoomItemWidget::setItemInteraction(int index) 
{
    connect(pickup.get(), &QAction::triggered,this,[this,index](){roomItemFunc(index);});
    itemMenu->popup(QCursor::pos());
}

void RoomItemWidget::listUpdated()
{
    for (int i=0;i < listWidget->count();i++) {
        if (listWidget->item(i)->isSelected()) {
            setItemInteraction(i);
        }
    }
}
