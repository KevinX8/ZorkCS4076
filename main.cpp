#include "zorkgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZorkGUI w;
    w.show();
    return a.exec();
}
