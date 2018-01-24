#include <QPushButton>
#include <QGridLayout>

#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{

    QGridLayout * layout = new QGridLayout(this);
    QLabel * top_label = new QLabel("Welcome to login!");
    top_label->setAlignment(Qt::AlignCenter);
    this->username_label = new QLabel("账号：");
    this->password_label = new QLabel("密码：");
    this->username_lineEdit = new QLineEdit;
    this->password_lineEdit = new QLineEdit;

    QPushButton * submit = new QPushButton("登录");

    layout->addWidget(top_label, 0, 0, 1, 3);
    layout->addWidget(this->username_label, 1, 0, 1, 1);
    layout->addWidget(this->username_lineEdit, 1, 1, 1, 2);

    layout->addWidget(this->password_label, 2, 0, 1, 1);
    layout->addWidget(this->password_lineEdit, 2, 1, 1, 2);

    layout->addWidget(submit, 3, 1, 1, 1);

    this->username_label->setAlignment(Qt::AlignCenter);
    this->password_label->setAlignment(Qt::AlignCenter);
    this->password_lineEdit->setEchoMode(QLineEdit::Password);

    this->error_dialog = new ErrorDialog;

    connect(submit, &QPushButton::clicked, [=]{

        if(!this->judge()){
            this->error_dialog->set_errorMessage("账号和密码都不能为空！");
            this->error_dialog->error_show();
        }else{
            emit this->signal_send();
        }
    });

    this->resize(300, 200);

    this->setWindowTitle("后台系统登录");

    this->setStyleSheet("QPushButton{"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                        "color:#fff;"
                        "font-size:14px;"
                        "padding:10px;"
                        "font-size:30px;"
                        "font-weight: 900;"
                        ""
                        "}"
                        "QLabel{"
                        "color:white;"
                        "background:gray;"
                        "padding: 10px;"
                        "font-size:18px;"
                        "border-radius: 8px;"
                        "}"
                        "QLineEdit{"
                        "background:rgba(0, 0, 0, 0.3);"
                        "color: #fff;"
                        "font-weight:900;"
                        "font-size: 20px;"
                        "padding:10px;"
                        "border-radius:6px;"
                        "border:2px solid gray;"
                        "}");

    top_label->setStyleSheet("background:none;color:blue;font-size:30px;font-weight:900;");

}

LoginWindow::~LoginWindow(){
    delete this->username_label;
    delete this->username_lineEdit;
    delete this->password_label;
    delete this->password_lineEdit;
    delete this->error_dialog;
}

QString LoginWindow::get_username(){
    return this->username_lineEdit->text().trimmed();
}

QString LoginWindow::get_password(){
    return this->password_lineEdit->text().trimmed();
}

bool LoginWindow::judge(){
   return !this->username_lineEdit->text().trimmed().isEmpty()
           &&
          !this->password_lineEdit->text().trimmed().isEmpty();

}


void LoginWindow::set_errorMessage(QString error_message){
    
   this->error_message = error_message;
}

void LoginWindow::slot_error(){
    this->error_dialog->set_errorMessage(this->error_message);
    this->error_dialog->error_show();
}




