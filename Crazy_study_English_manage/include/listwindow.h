#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QListWidget>

class ManageWindow;

class ListWindow : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWindow(QListWidget *parent = 0);

    void set_manageType(const QString & manage_type);
    void add_tableItem(const QString & table_name, const QString & table_name_show);
    void connect_sendAction(ManageWindow * manage_window);

private:
    QString manage_type;
    QStringList table_names;

signals:
    void signal_sendAction(const QString & table_name);

public slots:
};

#endif // LISTWINDOW_H
