#include "tablebutton.h"

TableButton::TableButton(QPushButton * parent) : QPushButton(parent)
{
    connect(this, &this->clicked, this, &this->slot_sendClick);
}

void TableButton::set_id(const int &id){
    this->id = id;
}

void TableButton::slot_sendClick(bool){
    emit this->signal_sendClick(this->id);
}
