#include "mainwindow.h"
#include "dyanmicWidget.h"
#include <QPushButton>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dynamicWidget dynamicWidget;

    QPushButton* btn1 = new QPushButton("A");
    QPushButton* btn2 = new QPushButton("B");
    QPushButton* btn3 = new QPushButton("C");

    dynamicWidget.addWidget(btn1);
    dynamicWidget.addWidget(btn2);
    dynamicWidget.addWidget(btn3);

    dynamicWidget.show();
    return a.exec();
}
