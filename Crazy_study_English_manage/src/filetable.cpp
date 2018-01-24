#include "filetable.h"

FileTable::FileTable(){
    this->clear();
}

void FileTable::clear(){
    this->set_manageType("");
    this->set_manageTypeShow("");
    this->set_tableName("");
    this->set_tableNameShow("");
    this->set_pageNum(0);
    this->set_rowLen(0);
    this->set_colLen(0);
    this->set_sum(0);
}

void FileTable::set_manageType(const QString &manage_type){
    this->manage_type = manage_type;
}

void FileTable::set_manageTypeShow(const QString &manage_type_show){
    this->manage_type_show = manage_type_show;
}

void FileTable::set_tableName(const QString &table_name){
    this->table_name = table_name;
}


void FileTable::set_tableNameShow(const QString &table_name_show){
    this->table_name_show = table_name_show;
}

void FileTable::set_fields(const QStringList &fields){
    this->fields = fields;
}


void FileTable::set_fieldsType(const QStringList &fields_type){
    this->fields_type = fields_type;
}


void FileTable::set_fieldsShow(const QStringList &fields_show){
    this->fields_show = fields_show;
}

void FileTable::set_fields_limit(const QStringList &fields_limit){
    this->fields_limit = fields_limit;
}

void FileTable::set_data(const QVector<QStringList> &data){
    this->data = data;
}


void FileTable::set_pageNum(const int &page_num){
    this->page_num = page_num;
}


void FileTable::set_sum(const int &sum){
    this->sum = sum;
}


void FileTable::set_rowLen(const int &row_len){
    this->row_len = row_len;
}


void FileTable::set_colLen(const int &col_len){
    this->col_len = col_len;
}


QString FileTable::get_manageType()const {
    return this->manage_type;
}

QString FileTable::get_manageTypeShow()const{
    return this->manage_type_show;
}


QString FileTable::get_tableName()const{
    return this->table_name;
}

QString FileTable::get_tableNameShow()const{
    return this->table_name_show;
}

QStringList FileTable::get_fields()const{
    return this->fields;
}


QStringList FileTable::get_fieldsType()const{
    return this->fields_type;
}


QStringList FileTable::get_fieldsShow()const{
    return this->fields_show;
}

QStringList FileTable::get_fieldsLimit()const{
    return this->fields_limit;
}

QVector<QStringList> FileTable::get_data()const{
    return this->data;
}


int FileTable::get_pageNum()const{
    return this->page_num;
}


int FileTable::get_sum()const{
    return this->sum;
}


int FileTable::get_rowLen()const{
    return this->row_len;
}


int FileTable::get_colLen()const{
    return this->col_len;
}

void FileTable::set_key(const QString &key){
    this->key = key;
}

void FileTable::set_value(const QString &value){
    this->value = value;
}

QString FileTable::get_key()const{
    return this->key;
}

QString FileTable::get_value()const{
    return this->value;
}
































