#ifndef __TEXTBOXWIDGET_H__
#define __TEXTBOXWIDGET_H__
#include <QLabel>
#include <QPushButton>
#include <vector>

using namespace std;

class GameInstance;

class TextBoxWidget : public QWidget {
    Q_OBJECT
    private:
    unique_ptr<QLabel> textBox;
    unique_ptr<QPushButton> button1;
    unique_ptr<QPushButton> button2;
    unique_ptr<QPushButton> button3;
    unique_ptr<QPushButton> button4;
    std::function<void()> funcBox1 = [](){return;};
    std::function<void()> funcBox2 = [](){return;};
    std::function<void()> funcBox3 = [](){return;};
    std::function<void()> funcBox4 = [](){return;};
    friend GameInstance;
    public:
    TextBoxWidget(QString text, vector<QString> interactionBoxes);
    void updateTextBox(QString text);
    void updateInteractions(vector<QString> interactionBoxes);
};

#endif // __TEXTBOXWIDGET_H__
