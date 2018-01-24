#ifndef TABLEBUTTON_H
#define TABLEBUTTON_H

#include <QObject>
#include <QPushButton>

class TableButton : public QPushButton
{

    Q_OBJECT
public:
    TableButton(QPushButton * parent = 0);

    void set_id(const int & id);

private:
    int id;
signals:
    void signal_sendClick(const int & id);
public slots:
    void slot_sendClick(bool);
};

#endif // TABLEBUTTON_H
