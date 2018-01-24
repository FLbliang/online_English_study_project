#include <QDebug>
#include "maincontroller.h"

#include "basewindow.h"

BaseWindow::BaseWindow(QWidget *parent) : QWidget(parent)
{

    this->configure = new Configure();
    this->manage_window = new ManageWindow();
    this->table_window = new TableWindow();
    this->data_show_window = new DataShowWindow();
    this->error_dialog = new ErrorDialog();
    this->error_dialog->setWindowTitle("Error!");
    this->layout = new QHBoxLayout(this);

    this->run_configure();
    this->create_manageWindow(this->configure->get_tables());
    this->layout->addWidget(this->manage_window);
    this->layout->addWidget(this->table_window);
    this->layout->setStretchFactor(this->manage_window, 1);
    this->layout->setStretchFactor(this->table_window, 4);

    connect(this->manage_window,
            &this->manage_window->signal_updateWindow,
            this,
            &this->slot_requestUpdateTableWindow);

    connect(this->table_window,
            &this->table_window->signal_sendDataShow,
            this,
            &this->slot_dataShow);

    this->resize(1200, 700);
    this->setWindowTitle("在线英语学习平台后台管理");

    this->setStyleSheet("background:#ccc;");
}

BaseWindow::~BaseWindow(){
    delete this->configure;
    delete this->manage_window;
    delete this->table_window;
    delete this->data_show_window;
    delete this->error_dialog;
    delete this->layout;
}

void BaseWindow::run_configure(){

    this->configure->set_fileName("sql_tables.txt");
    this->configure->explain_tables();
}

void BaseWindow::create_manageWindow(const QVector<FileTable> &tables){
    this->manage_window->create_listWindow(tables);

}

void BaseWindow::slot_requestUpdateTableWindow(const QString & table_name){
    emit this->table_window->signal_sendRequestPageData(table_name, 1, "", "");

}

void BaseWindow::slot_update_tableWindow(const FileTable &table){

    FileTable new_table = this->manage_window->get_table(table.get_tableName());
    new_table.set_rowLen(table.get_rowLen());
    new_table.set_data(table.get_data());
    new_table.set_sum(table.get_sum());
    new_table.set_pageNum(table.get_pageNum());
    new_table.set_colLen(new_table.get_fields().length());
    new_table.set_key(table.get_key());
    new_table.set_value(table.get_value());
    this->table_window->set_table(new_table);
    this->table_window->update_window();

}

void BaseWindow::slot_recvStatus(const QString &type, bool flag){

    if(type.compare("send error") == 0){
        this->error_dialog->set_errorMessage("网络数据发送出错，请检查网络连接!");
        this->error_dialog->error_show();

    }else if(type.compare("request_pageData_response") == 0 && !flag){
        this->error_dialog->set_errorMessage("请求页面数据失败或页面数据不存在!");
        this->error_dialog->error_show();

    }else if(type.compare("request_searchData_response") == 0 && !flag){
        this->error_dialog->set_errorMessage("查找的记录不存在!");
        this->error_dialog->error_show();

    }else if(type.compare("request_insert_response") == 0 && !flag){
        this->error_dialog->set_errorMessage("插入数据失败!");
        this->error_dialog->error_show();

    }else if(type.compare("request_update_response") == 0 && !flag){
        this->error_dialog->set_errorMessage("修改数据失败!");
        this->error_dialog->error_show();

    }else if(type.compare("request_delete_response") == 0 && !flag){
        this->error_dialog->set_errorMessage("删除数据失败!");
        this->error_dialog->error_show();

    }

    if(flag){

        FileTable table = this->table_window->get_table();
        emit this->table_window->signal_sendRequestPageData(
                    table.get_tableName(),
                    1,
                    this->table_window->get_table().get_key(),
                    this->table_window->get_table().get_value()
                    );

        this->data_show_window->close();

    }

}

void BaseWindow::slot_dataShow(const FileTable &table){

    this->data_show_window->create_window(table);
    this->data_show_window->show();

}

void BaseWindow::connect_requestPageData(MainController *main_controller){

    connect(
        this->table_window,
        &this->table_window->signal_sendRequestPageData,
        main_controller,
        &main_controller->slot_requestPageData
    );
}

void BaseWindow::connect_requestSearchData(MainController *main_controller){

    connect(
        this->table_window,
        &this->table_window->signal_sendRequestSearchData,
        main_controller,
        &main_controller->slot_requestSearchData
    );
}

void BaseWindow::connect_requestUpdata(MainController *main_controller){
    connect(
       this->data_show_window,
       &this->data_show_window->signal_sendRequestUpdate,
       main_controller,
       &main_controller->slot_requestUpdate
    );
}

void BaseWindow::connect_requestInsert(MainController *main_controller){

    connect(
       this->data_show_window,
       &this->data_show_window->signal_sendRequestInsert,
       main_controller,
       &main_controller->slot_requestInsert
    );
}

void BaseWindow::connect_requestDelete(MainController *main_controller){

    connect(
       this->table_window,
       &this->table_window->signal_sendRequestDelete,
       main_controller,
       &main_controller->slot_requestDelete
    );
}


















