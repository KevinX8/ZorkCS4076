#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "Floor.h"
#include <QPaintEvent>
#include <QWidget>

class MapWidget : public QWidget{
    Q_OBJECT
public:
    MapWidget();
protected:
    void paintEvent(QPaintEvent *event);
private:
    Floor f;
};

#endif // MAPWIDGET_H
