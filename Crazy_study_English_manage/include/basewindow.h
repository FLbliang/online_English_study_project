#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QWidget>
#include <QHBoxLayout>

#include "configure.h"
#include "managewindow.h"
#include "tablewindow.h"
#include "datashowwindow.h"
#include "errordialog.h"
#include "filetable.h"

class MainController;

class BaseWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWindow(QWidget *parent = 0);
    ~BaseWindow();

    void run_configure();
    void create_manageWindow(const QVector<FileTable> & tables);

    void connect_requestPageData(MainController * main_controller);
    void connect_requestSearchData(MainController *main_controller);
    void connect_requestUpdata(MainController * main_controller);
    void connect_requestInsert(MainController * main_controller);
    void connect_requestDelete(MainController * main_controller);


private:
    Configure * configure;
    ManageWindow * manage_window;
    TableWindow * table_window;
    DataShowWindow * data_show_window;
    ErrorDialog * error_dialog;

    QHBoxLayout * layout;

signals:

public slots:
    void slot_dataShow(const FileTable & table);
    void slot_recvStatus(const QString & type, bool flag);
    void slot_requestUpdateTableWindow(const QString & table_name);
    void slot_update_tableWindow(const FileTable & table);
};

#endif // BASEWINDOW_H
