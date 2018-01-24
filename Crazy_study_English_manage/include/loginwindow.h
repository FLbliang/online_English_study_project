#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>

#include "errordialog.h"

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

    QString get_username();
    QString get_password();
    bool judge();
    void set_errorMessage(QString error_message);


private:
    QLabel * username_label;
    QLineEdit * username_lineEdit;
    QLabel * password_label;
    QLineEdit * password_lineEdit;
    ErrorDialog * error_dialog;
    QString error_message;

signals:
    void signal_send();

public slots:
    void slot_error();
};

#endif // LOGINWINDOW_H
