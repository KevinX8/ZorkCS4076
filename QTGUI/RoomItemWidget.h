#ifndef __ROOMITEMWIDGET_H__
#define __ROOMITEMWIDGET_H__
#include "..\GameWorld\GameInstance.h"
#include <QListView>

class RoomItemWidget {
    private:
    QList<QAction> roomItems;
    public:
    QListView listView;
    RoomItemWidget(vector<int> itemCodes);
    void updateItems(int roomIndex);
}

#endif // __ROOMITEMWIDGET_H__