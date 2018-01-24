#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>

#include "loginwindow.h"
#include "basewindow.h"
#include "socketcontroller.h"
#include "dataexplain.h"

class FileTable;

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();

    void transform_window();

private:
    LoginWindow * login_window;
    BaseWindow * base_window;
    SocketController * socket_controller;
    DataExplain * data_explain;
signals:
    void signal_errLogin();
    void signal_recvData(const FileTable & table);
    void signal_recvStatus(const QString & type, bool flag);

public slots:
    void slot_order();
    void slot_recvLogin();
    void slot_requestPageData(
            const QString & table_name,
            const int & page_num,
            const QString & key,
            const QString & value);

    void slot_requestSearchData(
            const QString & table_name,
            const QString & key,
            const QString & value);

    void slot_requestUpdate(
            const QString & table_name,
            const QStringList & fields,
            const QStringList & old_values,
            const QStringList & new_values
            );

    void slot_requestInsert(
            const QString & table_name,
            const QStringList & fields,
            const QStringList & values);

    void slot_requestDelete(
            const QString & table_name,
            const QStringList & fields,
            const QVector<QStringList> & values
            );
};

#endif // MAINCONTROLLER_H
