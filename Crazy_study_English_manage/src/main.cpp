#include "widget.h"
#include <QApplication>

#include "maincontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainController main_controller;

    return a.exec();
}
