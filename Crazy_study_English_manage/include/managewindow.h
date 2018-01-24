#ifndef MANAGEWINDOW_H
#define MANAGEWINDOW_H

#include <QHBoxLayout>
#include <QWidget>
#include <QToolBox>
#include <QMap>

#include "filetable.h"

class ManageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ManageWindow(QWidget *parent = 0);
    ~ManageWindow();

    void create_listWindow(const QVector<FileTable> & tables);

    FileTable get_table(const QString & table_name);


private:
    QToolBox * toolBox;

    QMap<QString, FileTable> map_table;

signals:
    void signal_updateWindow(const QString & table_name);

public slots:
    void slot_itemAction(const QString & table_name);
};

#endif // MANAGEWINDOW_H
