#include <QException>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include "dataexplain.h"

DataExplain::DataExplain(QObject *parent) : QObject(parent)
{

}

void DataExplain::response_error(){

    this->type = "response_error";
    this->status = 0;
}

void DataExplain::explain_data(const QString &recv_data){

    try{

       QJsonParseError parse_error;
       QJsonDocument document = QJsonDocument::fromJson(recv_data.toUtf8(), &parse_error);

       if(!document.isNull() && parse_error.error == QJsonParseError::NoError){

           if(document.isObject()){

               QJsonObject object = document.object();

               if(object.contains("response")){

                    this->type = object.value("response").toString();
               }else{
                   this->response_error();
                   return;
               }

               if(object.contains("status")){
                   this->status = object.value("status").toString().toInt();
               }else{
                   this->response_error();
                   return;
               }

              if(this->type.compare("request_login_response") == 0) {

                  if(this->status){

                    if(object.contains("code")){
                        this->connect_code = object.value("code").toString();

                    }else{
                        this->response_error();
                        return;
                    }
                  }

              }else if(this->type.compare("request_pageData_response") == 0
                       || this->type.compare("request_searchData_response") == 0){

                 QVector<QStringList> values;
                 if(this->status){
                     QJsonValue json_value = object.value("data");
                     int result_sum = object.value("result_sum").toString().toInt();

                     if(json_value.isArray()){

                         QJsonArray data_array = json_value.toArray();
                         for(QJsonValue json_value : data_array){

                             if(json_value.isArray()){

                                 QJsonArray data_item = json_value.toArray();
                                 QStringList item_values;

                                 for(QJsonValue data_value : data_item){
                                     item_values << data_value.toString();
                                 }
                                 values.push_back(item_values);

                             }else{
                                this->response_error();
                                 return;
                             }
                         }

                         this->table.set_data(values);
                         this->table.set_sum(result_sum);
                         this->table.set_rowLen(values.length());

                     }else{
                         this->response_error();
                         return;
                     }
                 }else{
                     this->table.set_data(values);
                     this->table.set_sum(0);
                     this->table.set_rowLen(0);

                 }

           }else if(this->type.compare("request_delete_response") == 0
                    || this->type.compare("request_update_response") == 0
                    || this->type.compare("request_insert_response") == 0){

                 return ;

           }else{
                  this->response_error();
              }

         }else{
             this->response_error();
          }
       }else{
           this->response_error();
       }

    }catch(...){
        this->response_error();
    }

}

QByteArray DataExplain::transform_login(const QString &username, const QString &password){

    QJsonObject json;
    json.insert("order", "request_login");
    json.insert("username", username);
    json.insert("password", password);

    QJsonDocument document;
    document.setObject(json);

    return document.toJson();
}

QByteArray DataExplain::transform_requestData(const QString &table_name,
                                           const int & page_num,
                                           const QString & key,
                                           const QString & value){

    QJsonObject json;
    QJsonDocument document;

    json.insert("order", "request_pageData");
    json.insert("code", this->connect_code);
    json.insert("table_name", table_name);
    json.insert("page_num", QString::number(page_num));


    if(!key.isEmpty() && !value.isEmpty()){
        json.insert("key", key);
        json.insert("value", value);
    }

    this->table.set_tableName(table_name);
    this->table.set_pageNum(page_num);
    this->table.set_key(key);
    this->table.set_value(value);

    document.setObject(json);
    return document.toJson();
}

QByteArray DataExplain::transform_requestDelete(
        const QString &table_name,
        const QStringList &fields,
        const QVector<QStringList> &values)
{

    QJsonObject json;
    QJsonDocument document;
    QJsonArray data;

    json.insert("order", "request_delete");
    json.insert("table_name", table_name);
    json.insert("code", this->connect_code);
    QVector<QStringList>::const_iterator values_pointer;

    for(values_pointer = values.begin(); values_pointer != values.end(); ++values_pointer){

        QJsonObject data_item;
        int coord = 0;
        for(QString value : *values_pointer){
            data_item.insert(fields.at(coord++), value);
        }

        data.append(data_item);
    }
    json.insert("data", data);
    document.setObject(json);

    return document.toJson();
}

QByteArray DataExplain::transform_requestData(
        const QString &table_name,
        const QString &key,
        const QString &value)

{

    QJsonObject json;
    QJsonDocument document;

    json.insert("order", "request_searchData");
    json.insert("code", this->connect_code);
    json.insert("table_name", table_name);
    json.insert("key", key);
    json.insert("value", value);

    this->table.set_tableName(table_name);
    this->table.set_key(key);
    this->table.set_value(value);

    document.setObject(json);

    return document.toJson();
}

QByteArray DataExplain::transform_requestInsert(
        const QString &table_name,
        const QStringList &fields,
        const QStringList &values)
{
    QJsonObject json;
    QJsonDocument document;
    QJsonObject data;

    json.insert("order", "request_insert");
    json.insert("table_name", table_name);
    json.insert("code", this->connect_code);

    int coord = 0;
    for(QString value : values){
        if(value.compare("系统自动创建") == 0){
            value = "";
        }
        data.insert(fields.at(coord++), value);
    }

    json.insert("data", data);
    document.setObject(json);

    return document.toJson();
}

QByteArray DataExplain::transform_requestUpdate(
        const QString &table_name,
        const QStringList &fields,
        const QStringList &old_values,
        const QStringList &new_values)
{

    QJsonObject json;
    QJsonDocument document;
    QJsonObject old_data;
    QJsonObject new_data;

    json.insert("order", "request_update");
    json.insert("table_name", table_name);
    json.insert("code", this->connect_code);

    int coord = 0;
    for(QString old_value : old_values){
        old_data.insert(fields.at(coord++), old_value);
    }

    coord = 0;
    for(QString new_value : new_values){
        new_data.insert(fields.at(coord++), new_value);
    }

    json.insert("old_data", old_data);
    json.insert("new_data", new_data);
    document.setObject(json);

    return document.toJson();

}


QString DataExplain::get_type(){
    return this->type;
}


int DataExplain::get_status(){
    return this->status;
}


FileTable DataExplain::get_table(){
    return this->table;
}















