#ifndef __TEXTBOXWIDGET_H__
#define __TEXTBOXWIDGET_H__
#include "..\GameWorld\GameInstance.h"
#include <QLabel>
#include <QPushButton>

class TextBoxWidget : public QWidget {
    Q_OBJECT
    private:
    QLabel textBox;
    QPushButton button1;
    QPushButton button2;
    QPushButton button3;
    QPushButton button4;
    public:
    TextBoxWidget(string text, vector<string> interactionBoxes);
    void updateTextBox(string text);
    void updateInteractions(vector<string> interactionBoxes);
}

#endif // __TEXTBOXWIDGET_H__