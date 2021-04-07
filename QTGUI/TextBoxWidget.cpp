#include "TextBoxWidget.h"

TextBoxWidget::TextBoxWidget(QString text, vector<QString> interactionBoxes)
{
    funcBox1 = [](){return;};
    funcBox2 = [](){return;};
    funcBox3 = [](){return;};
    funcBox4 = [](){return;};
    textBox = unique_ptr<QLabel>(new QLabel(text,this));
    button1 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[0],this));
    button2 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[1],this));
    button3 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[2],this));
    button4 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[3],this));
    QObject::connect(button1.get(),&QPushButton::released, this, [this](){TextBoxWidget::funcBox1();});
    QObject::connect(button2.get(),&QPushButton::released, this, [this](){TextBoxWidget::funcBox2();});
    QObject::connect(button3.get(),&QPushButton::released, this, [this](){TextBoxWidget::funcBox3();});
    QObject::connect(button4.get(),&QPushButton::released, this, [this](){TextBoxWidget::funcBox4();});
}
void TextBoxWidget::updateTextBox(QString text)
{
    textBox->setText(text);
}

void TextBoxWidget::updateInteractions(vector<QString> interactionBoxes)
{
    button1->setText(interactionBoxes[0]);
    button2->setText(interactionBoxes[1]);
    button3->setText(interactionBoxes[2]);
    button4->setText(interactionBoxes[3]);
}

