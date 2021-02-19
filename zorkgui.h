#ifndef ZORKGUI_H
#define ZORKGUI_H

#include <QMainWindow>
#include "Floor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ZorkGUI; }
QT_END_NAMESPACE

class ZorkGUI : public QMainWindow
{
    Q_OBJECT

public:
    ZorkGUI(QWidget *parent = nullptr);
    ~ZorkGUI();
    void drawFloor(Floor *floor);

private:
    Ui::ZorkGUI *ui;
};
#endif // ZORKGUI_H
