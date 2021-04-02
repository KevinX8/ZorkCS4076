#include "TextBoxWidget.h"

TextBoxWidget::TextBoxWidget(string text, vector<string> interactionBoxes, GameInstance &instance) 
{
    textBox = QLabel(text,this);
    button1 = QPushButton(interactionBoxes[0],this);
    button2 = QPushButton(interactionBoxes[1],this);
    button3 = QPushButton(interactionBoxes[2],this);
    button4 = QPushButton(interactionBoxes[3],this);
    QObject::connect(button1,&QPushButton::released, this, &instance.funcBox1());
    QObject::connect(button2,&QPushButton::released, this, &instance.funcBox2());
    QObject::connect(button3,&QPushButton::released, this, &instance.funcBox3());
    QObject::connect(button4,&QPushButton::released, this, &instance.funcBox4());
}
void TextBoxWidget::updateTextBox(string text) 
{
    textBox.setText(text);
}

void TextBoxWidget::updateInteractions(vector<string> interactionBoxes) 
{
    button1.setText(interactionBoxes[0]);
    button2.setText(interactionBoxes[1]);
    button3.setText(interactionBoxes[2]);
    button4.setText(interactionBoxes[3]);
}

