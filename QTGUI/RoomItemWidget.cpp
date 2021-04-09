 #include "RoomItemWidget.h"

RoomItemWidget::RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc)
{
    this->roomItemFunc = roomItemFunc;
    itemMenu = unique_ptr<QMenu>(new QMenu(this));
    pickup = unique_ptr<QAction>(new QAction("Pickup",this));
    close = unique_ptr<QAction>(new QAction("Close",this));
    roomItemsLabel = unique_ptr<QLabel>(new QLabel(QString::fromStdString("Items in Room: 0"),this));
    listWidget = new QListWidget(this);
    itemMenu->addAction(pickup.get());
    itemMenu->addAction(close.get());
    connect(close.get(), &QAction::triggered,this,[this](){itemMenu->hide();});
    connect(listWidget,&QListWidget::itemChanged,this,[this](){this->listUpdated();});
    this->updateItems(itemCodes);
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    int index = 0;
    roomItemsLabel->clear();
    for (int itemCode: itemCodes) {
        listWidget->addItem(QString::fromStdString(Item::getType(itemCode) + Item::itemNameMap.at(itemCode)));
        index++;
    }
    roomItemsLabel->setText(QString::fromStdString("Items in Room: " + to_string(index)));
}

void RoomItemWidget::setItemInteraction(int index) 
{
    connect(pickup.get(), &QAction::triggered,this,[this,index](){roomItemFunc(index);});
    itemMenu->popup(QWidget::mapFromGlobal(QCursor::pos()));
}

void RoomItemWidget::listUpdated()
{
    for (int i=0;i < listWidget->count();i++) {
        if (listWidget->item(i)->isSelected()) {
            setItemInteraction(i);
        }
    }
}
