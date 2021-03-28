
#include "RoomItemWidget.h"
RoomItemWidget::RoomItemWidget(vector<int> itemCodes) : listView(Qwidget* MainWindow)
{
    this->updateItems(itemCodes);
    listView.addActions(roomItems);
}

void RoomItemWidget::updateItems(vector<int> itemCodes) 
{
    roomItems.clear();
    for (int itemCode: itemIndexes) {
        QAction itemAction = QAction(Item::getType(itemCode) + Item::itemNameMap.at(itemCode));
        connect(itemAction, &QAction::)
        roomItems.push_back(itemAction);
    }
}
