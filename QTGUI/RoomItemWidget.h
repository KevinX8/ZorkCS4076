#ifndef __ROOMITEMWIDGET_H__
#define __ROOMITEMWIDGET_H__
#include "..\GameWorld\GameInstance.h"
#include <QListView>
#include <QMenu>
#include <QAction>

class RoomItemWidget : public QWidget {
    Q_OBJECT
    private:
    QList<QAction> roomItems;
    QMenu itemMenu;
    GameInstance &instance;
    void setItemInteraction(int itemCode);
    public:
    QListView listView;
    RoomItemWidget(vector<int> itemCodes, GameInstance &instance);
    void updateItems(int roomIndex);
}



#endif // __ROOMITEMWIDGET_H__