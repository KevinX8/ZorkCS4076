#include "TextBoxWidget.h"

TextBoxWidget::TextBoxWidget(QString text, vector<QString> interactionBoxes)
{
    //sets the default function of the bottom 4 interaction boxes to safe functions
    funcBox1 = [](){return;};
    funcBox2 = [](){return;};
    funcBox3 = [](){return;};
    funcBox4 = [](){return;};
    //fills the giant textbox in the bottom middle of the screen 
    textBox = unique_ptr<QLabel>(new QLabel(text,this));
    //sets the text of each interaction button
    button1 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[0],this));
    button2 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[1],this));
    button3 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[2],this));
    button4 = unique_ptr<QPushButton>(new QPushButton(interactionBoxes[3],this));
    //makes the big textbox white for better readability
    QPalette pal = textBox->palette();
    pal.setColor(QPalette::Window, QColor(Qt::white));
    textBox->setAutoFillBackground(true);
    textBox->setPalette(pal);
    textBox->update();
    //makes the text centred horizontally and sets the vertical and horizontal margin to 50
    textBox->setMargin(50);
    textBox->setAlignment(Qt::AlignHCenter);
    //size out each button appropriately and moves them to the correct positions
    button1->setGeometry(30,330-120,200,30);
    button2->setGeometry(button1->x() + button1->width()+10,330-120,200,30);
    button3->setGeometry(button2->x() + button2->width()+10,330-120,200,30);
    button4->setGeometry(button3->x() + button3->width()+10,330-120,200,30);
    //connects each button to the function pointers
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

void TextBoxWidget::enableButtons(int amount) 
{
    //enables "amount" amount of buttons and disables the rest of them
    int count = 0;
    while(count < amount){
        switch (count) {
            case 0: {button1->setEnabled(true); break;}
            case 1: {button2->setEnabled(true); break;}
            case 2: {button3->setEnabled(true); break;}
            case 3: {button4->setEnabled(true); break;}
        }
        count++;
    }
    while (count < 4){
        switch (count) {
            case 0: {button1->setEnabled(false); break;}
            case 1: {button2->setEnabled(false); break;}
            case 2: {button3->setEnabled(false); break;}
            case 3: {button4->setEnabled(false); break;}
        }
        count++;
    }
}

