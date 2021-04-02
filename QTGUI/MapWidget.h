#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "../GameWorld/Floor.h"
#include "../GameWorld/GameInstance.h"
#include "../GameWorld/Tools.h"
#include <QPaintEvent>
#include <QWidget>
#include <QPainter>
#include <QIcon>
#include <QPushButton>
#include <QGraphicsScene>

class MapWidget : public QGraphicsScene{
    Q_OBJECT
public:
    MapWidget(int startIndex, GameInstance& g, QObject *parent = nullptr);;
protected:
    void paintEvent(QPaintEvent *event);
    void drawWalls(QPainter *qp);
    void changeRoom(int roomIndex);
private:
    void resetButtons();
    Floor f;
    Room& current;
    GameInstance& game;
    vector<QPushButton> npcButtons;
};

#endif // MAPWIDGET_H

