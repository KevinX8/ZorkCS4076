#include <QApplication>
#include <QGridLayout>
#include "QTGUI\MapWidget.h"
#include <QWidget>

int versionCODEALPHAOMEGABETA = 1;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

      MapWidget window;

      window.resize(500, 600);
      window.setWindowTitle("Zork");
      window.show();

      return app.exec();
}
