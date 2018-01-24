#include "managewindow.h"
#include "listwindow.h"

ListWindow::ListWindow(QListWidget *parent) : QListWidget(parent)
{

}

void ListWindow::set_manageType(const QString &manage_type){

    this->manage_type = manage_type;
}

void ListWindow::add_tableItem(const QString &table_name, const QString & table_name_show){

    this->table_names << table_name;
    this->addItem(table_name_show);

}

void ListWindow::connect_sendAction(ManageWindow *manage_window){

    connect(this, &this->signal_sendAction, manage_window, &manage_window->slot_itemAction);

    connect(this, &this->itemClicked, [=]{

        emit this->signal_sendAction(this->table_names.at(this->currentRow()));
    });
}

