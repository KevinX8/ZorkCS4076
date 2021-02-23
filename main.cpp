#include "zorkgui.h"

#include <QApplication>
#include <QGridLayout>
#include "MapWidget.h"
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    //ZorkGUI game;
    //game.show();
    QGridLayout *gridLayout = new QGridLayout;
    MapWidget mapWidget = MapWidget();
    //gridLayout->addWidget(mapWidget, 0, 0, 10, 10);

    return program.exec();
}
