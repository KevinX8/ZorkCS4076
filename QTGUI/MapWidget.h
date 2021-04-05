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

class GameInstance;

class MapWidget : public QGraphicsScene{
    Q_OBJECT
public:
    MapWidget(int startIndex, GameInstance& g, QObject *parent = nullptr);
    void resetButtons();
    void changeRoom(int roomIndex);
protected:
    void paintEvent(QPaintEvent *event);
    void drawWalls(QPainter *qp);
private:
    Floor f;
    Room& current;
    GameInstance& game;
    vector<QPushButton> npcButtons;
    vector<QPushButton> doorButtons;
};

#endif // MAPWIDGET_H

