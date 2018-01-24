#include <QVBoxLayout>
#include <QVector>

#include "managewindow.h"
#include "filetable.h"
#include "listwindow.h"

ManageWindow::ManageWindow(QWidget *parent) : QWidget(parent)
{
    this->toolBox = new QToolBox();

    QVBoxLayout * layout = new QVBoxLayout(this);

    layout->addWidget(this->toolBox);
    layout->addStretch(10);
}

ManageWindow::~ManageWindow(){
    delete this->toolBox;
}

void ManageWindow::create_listWindow(const QVector<FileTable> &tables){

    QVector<FileTable>::const_iterator p;
    QMap<QString, int> map_listWindow;

    QVector<ListWindow *> list_windows;

    ListWindow * list_window;

    for(p = tables.begin(); p != tables.end(); ++p){

        int coord = map_listWindow[p->get_manageType()];

        if(coord == 0){

            list_window = new ListWindow();
            list_window->set_manageType(p->get_manageType());
            list_window->connect_sendAction(this);
            list_window->add_tableItem(p->get_tableName(), p->get_tableNameShow());

            list_windows.push_back(list_window);
            map_listWindow[p->get_manageType()] = list_windows.length();

            this->toolBox->addItem(list_window, p->get_manageTypeShow());

        }else{

            list_window = *(list_windows.begin() + coord - 1);
            list_window->add_tableItem(p->get_tableName(), p->get_tableNameShow());

        }

        this->map_table[p->get_tableName()] = *p;

    }

    this->setStyleSheet("QListWidget{"
                        "background: #ccc;"
                        "color:black;"
                        "font-size:18px;"
                        "font-weight: 700;"
                        "}"
                        "QToolBox::Tab{"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                        "color:orange;"
                        "font-size:20px;"
                        "paddint:15px 0px;"
                        "font-weight:900;"
                        "}"
                        "QScrollBar:horizontal {"
                       " height:10px; "
                       " background-color:rgba(0,0,0,0%); "
                       " padding-left:10px; padding-right:10px;"
                   " }");

}

FileTable ManageWindow::get_table(const QString &table_name){

     return this->map_table[table_name];
}

void ManageWindow::slot_itemAction(const QString &table_name){

   emit this->signal_updateWindow(table_name);
}


