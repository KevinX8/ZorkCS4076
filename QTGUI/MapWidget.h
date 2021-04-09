#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "../GameWorld/Floor.h"
#include "../GameWorld/Tools.h"
#include <QPaintEvent>
#include <QWidget>
#include <QPainter>
#include <QIcon>
#include <QPushButton>
#include <QGraphicsScene>
#include <QString>
#include <QPalette>
#include <unordered_set>

namespace QTGui {
    class MapWidget;
}

using namespace QTGui;

class QTGui::MapWidget : public QWidget{
    Q_OBJECT
public:
    MapWidget(int i, Floor& f, Room& r, std::function<void(Door&)>& doorFunc, std::function<void(shared_ptr<NPC>)>& npcFunc, std::function<void()>& resetFunc, std::function<void(bool)>& floorFunc, QWidget *parent = 0);
    void resetButtons();
    void changeRoom(Room& room);
    void removeNPC(shared_ptr<NPC> npc);
    int xOffset = 0;
protected:
    void paintEvent(QPaintEvent *event);
    void drawWalls(QPainter *qp);
private:
    friend class GameInstance;
    Room current;
    int currentRoomIndex;
    Floor& f;
    vector<shared_ptr<QPushButton>> npcButtons;
    vector<shared_ptr<QPushButton>> doorButtons;
    std::function<void(Door&)> doorFunc;
    std::function<void(shared_ptr<NPC>)> npcFunc;
    std::function<void()> resetFunc;
    std::function<void(bool)> floorFunc;
};

#endif // MAPWIDGET_H

