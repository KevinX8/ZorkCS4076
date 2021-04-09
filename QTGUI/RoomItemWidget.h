#ifndef __ROOMITEMWIDGET_H__
#define __ROOMITEMWIDGET_H__
#include <QListWidget>
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

namespace QTGui {
    class RoomItemWidget;
}

using namespace QTGui;

class QTGui::RoomItemWidget : public QWidget {
    Q_OBJECT
    private:
    unique_ptr<QLabel> roomItemsLabel;
    unique_ptr<QMenu> itemMenu;
    unique_ptr<QAction> pickup;
    unique_ptr<QAction> close;
    std::function<void(int)> roomItemFunc;
    void setItemInteraction(int itemCode);
    void listUpdated();
    public:
    QListWidget* listWidget;
    RoomItemWidget(vector<int> itemCodes, std::function<void(int)> roomItemFunc);
    ~RoomItemWidget();
    void updateItems(vector<int> itemCodes);
};



#endif // __ROOMITEMWIDGET_H__
