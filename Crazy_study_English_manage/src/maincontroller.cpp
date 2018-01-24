#include "maincontroller.h"

MainController::MainController(QObject *parent) : QObject(parent)
{
    this->login_window = new LoginWindow();
    this->base_window = new BaseWindow();
    this->socket_controller = new SocketController();
    this->data_explain = new DataExplain();

    this->login_window->show();

    // 登录模块信号和槽绑定
    connect(this->login_window, &this->login_window->signal_send, this, &this->slot_recvLogin);
    connect(this, &this->signal_errLogin, this->login_window, &this->login_window->slot_error);

    // 请求页面数据信号和槽绑定
    this->base_window->connect_requestPageData(this);

    // 请求查询数据信号和槽绑定
    this->base_window->connect_requestSearchData(this);

    // 请求更新数据信号和槽绑定
    this->base_window->connect_requestUpdata(this);

    // 请求增加数据信号和槽绑定
    this->base_window->connect_requestInsert(this);

    // 请求删除数据信号和槽绑定
    this->base_window->connect_requestDelete(this);

    // 返回请求数据的信号和槽绑定
    connect(this, &this->signal_recvData, this->base_window, &this->base_window->slot_update_tableWindow);

    // 返回状态数据通知的信号和槽绑定
    connect(this, &this->signal_recvStatus, this->base_window, &this->base_window->slot_recvStatus);

    connect(this->socket_controller, &this->socket_controller->signal_hadRead, this, &this->slot_order);

}

MainController::~MainController(){

    delete this->login_window;
    delete this->base_window;
    delete this->socket_controller;
    delete this->data_explain;
}

void MainController::transform_window(){
    this->login_window->close();
    this->base_window->show();
}

void MainController::slot_order(){

    QString recv_data = this->socket_controller->get_recvData();
    this->data_explain->explain_data(recv_data);

    QString type = this->data_explain->get_type();
    int status = this->data_explain->get_status();

    if(type.compare("explain_error") == 0){

        emit this->signal_recvStatus("explain error", false);
        return ;
    }

        if(status == 0){
           if(type.compare("request_login_response") == 0){
               this->login_window->set_errorMessage("用户名或密码错误!");
               emit this->signal_errLogin();
           }else{
               emit this->signal_recvStatus(type, false);

               if(type.compare("request_pageData_response") == 0 || type.compare("request_searchData_response") == 0){

                    emit this->signal_recvData(this->data_explain->get_table());
               }
           }
        }else{
            if(type.compare("request_login_response") == 0){
               this->transform_window();
                return;
            }else if(type.compare("request_pageData_response") == 0 || type.compare("request_searchData_response") == 0){
                emit this->signal_recvData(this->data_explain->get_table());
            }else{

                emit this->signal_recvStatus(type, true);
            }

        }
}

void MainController::slot_recvLogin(){
    //test
    //this->transform_window();
   // return ;

    QString command_str = this->data_explain->transform_login(
                this->login_window->get_username(),
                this->login_window->get_password());


    if( !this->socket_controller->send_message(command_str) ){
       this->login_window->set_errorMessage("网络出错，请检查网络!");
       emit this->signal_errLogin();
       return;
    }


}

void MainController::slot_requestPageData(
        const QString &table_name,
        const int &page_num,
        const QString & key,
        const QString & value)

{
    QString command_str(this->data_explain->transform_requestData(table_name, page_num, key, value));

    if(!this->socket_controller->send_message(command_str)){
       emit this->signal_recvStatus("send error", false);
       return ;
    }



}

void MainController::slot_requestSearchData(
        const QString &table_name,
        const QString &key,
        const QString &value)

{
    QString command_str(this->data_explain->transform_requestData(table_name, key, value));

    if(!this->socket_controller->send_message(command_str)){
       emit this->signal_recvStatus("send error", false);
       return ;
    }


}

void MainController::slot_requestDelete(
        const QString &table_name,
        const QStringList &fields,
        const QVector<QStringList> &values)

{
    QString command_str(this->data_explain->transform_requestDelete(table_name, fields, values));

    if(!this->socket_controller->send_message(command_str)){
       emit this->signal_recvStatus("send error", false);
       return ;
    }


}

void MainController::slot_requestInsert(
        const QString &table_name,
        const QStringList &fields,
        const QStringList &values)
{
    QString command_str(this->data_explain->transform_requestInsert(table_name, fields, values));

    if(!this->socket_controller->send_message(command_str)){
       emit this->signal_recvStatus("send error", false);
       return ;
    }


}

void MainController::slot_requestUpdate(
        const QString &table_name,
        const QStringList &fields,
        const QStringList &old_values,
        const QStringList &new_values)
{
    QString command_str(this->data_explain->transform_requestUpdate(table_name, fields, old_values, new_values));

    if(!this->socket_controller->send_message(command_str)){
       emit this->signal_recvStatus("send error", false);
       return ;
    }


}













