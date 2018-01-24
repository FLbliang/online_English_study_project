#include "widget.h"
#include "ui_widget.h"

#include "loginwindow.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    LoginWindow * login_window = new LoginWindow;

    this->close();
    login_window->show();
}

Widget::~Widget()
{
    delete ui;
}
