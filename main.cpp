#include <QApplication>
#include "GameWorld/GameInstance.h"

int versionCODEALPHAOMEGABETA = 1;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GameInstance instance = GameInstance();
    //instance.changeFloor(true);
      //MapWidget window;

      //window.resize(500, 600);
      //window.setWindowTitle("Zork");
      //window.show();
      return app.exec();
}
