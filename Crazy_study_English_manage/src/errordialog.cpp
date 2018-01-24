#include <QLabel>
#include <QPushButton>

#include "errordialog.h"

ErrorDialog::ErrorDialog(QDialog *parent) : QDialog(parent)
{

    this->setWindowTitle("Login error");
    this->error_message_label = new QLabel;
    this->close_btn = new QPushButton("知道啦!");
    this->layout = new QVBoxLayout(this);

    this->layout->addWidget(this->error_message_label);
    this->layout->addWidget(this->close_btn);

    connect(this->close_btn, &QPushButton::clicked, this, &QDialog::close);

    this->resize(200, 200);

    this->setStyleSheet(""
                        "QLabel{"
                        "color:red;"
                        "font-size:25px;"
                        "font-weight:900;"
                        "background:#ccc;"
                        "border-radius:30px;"
                        "}"
                        "QPushButton{"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929) !important;"
                        "color:#fff;"
                        "font-size:22px;"
                        "font-weight:900;"
                        "padding:20px;"
                        "}");

}

void ErrorDialog::error_show(){

    this->error_message_label->setText(this->error_message);
    this->show();
}


void ErrorDialog::set_errorMessage(const QString & error_message){

    this->error_message = error_message;
}


