#ifndef __ROOMITEMWIDGET_H__
#define __ROOMITEMWIDGET_H__
#include <QListView>
#include <QMenu>
#include <QLabel>
#include <QList>
#include <QAction>
#include <vector>
#include <QWidget>
#include <memory>
#include <functional>
#include "../interaction/Item/item.h"

using namespace std;

class RoomItemWidget : public QWidget {
    Q_OBJECT
    private:
    QList<QAction*> roomItems;
    unique_ptr<QLabel> roomItemsLabel;
    unique_ptr<QMenu> itemMenu;
    unique_ptr<QAction> pickup;
    unique_ptr<QAction> close;
    void setItemInteraction(int itemCode);
    std::function<void(int)> roomItemFunc;
    public:
    QListView listView;
    RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc);
    void updateItems(vector<int> itemCodes);
};



#endif // __ROOMITEMWIDGET_H__
