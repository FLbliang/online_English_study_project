#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QString>

class ErrorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ErrorDialog(QDialog *parent = 0);

    void error_show();
    void set_errorMessage(const QString & error_message);

private:
    QString error_message;

    QLabel * error_message_label;
    QPushButton * close_btn;
    QVBoxLayout * layout;

signals:

public slots:
};

#endif // ERRORDIALOG_H
