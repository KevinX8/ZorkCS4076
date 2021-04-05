#ifndef __ROOMITEMWIDGET_H__
#define __ROOMITEMWIDGET_H__
#include <QListView>
#include <QMenu>
#include <QList>
#include <QAction>
#include <vector>
#include <QWidget>

using namespace std;

class GameInstance;

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
    void updateItems(vector<int> itemCodes);
};



#endif // __ROOMITEMWIDGET_H__
