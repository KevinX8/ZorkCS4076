#ifndef ZORKGUI_H
#define ZORKGUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ZorkGUI; }
QT_END_NAMESPACE

class ZorkGUI : public QMainWindow
{
    Q_OBJECT

public:
    ZorkGUI(QWidget *parent = nullptr);
    ~ZorkGUI();

private:
    Ui::ZorkGUI *ui;
};
#endif // ZORKGUI_H
