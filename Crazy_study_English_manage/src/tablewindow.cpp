#include <QCheckBox>

#include "tablebutton.h"
#include "tablewindow.h"

TableWindow::TableWindow(QWidget *parent) : QWidget(parent)
{
    QGridLayout * grid_layout = new QGridLayout(this);

    QLabel * main_label = new QLabel("后台管理");
    main_label->setAlignment(Qt::AlignCenter);
    main_label->setStyleSheet("QLabel{"
                              "color:#909090;"
                              "font-size:100px;"
                              "font-weight:900;"
                              "}");

    grid_layout->addWidget(main_label, 0, 0);
    record_labels.push_back(main_label);
    record_gridLayouts.push_back(grid_layout);

    this->select_comboBox = NULL;
    this->tableWidget = NULL;
    this->main_layout = NULL;



}

void TableWindow::clear_window(){

    QVector<QLabel * >::iterator label_pointer;
    QVector<QLineEdit * >::iterator lineEdit_pointer;
    QVector<QTextEdit * >::iterator textEdit_pointer;
    QVector<QPushButton * >::iterator button_pointer;
    QVector<QGridLayout * >::iterator gridLayout_pointer;
    QVector<QVBoxLayout * >::iterator vLayout_pointer;
    QVector<QHBoxLayout * >::iterator hLayout_pointer;
    QVector<QCheckBox * >::iterator checkBox_pointer;

    for(label_pointer = this->record_labels.begin();
        label_pointer != this->record_labels.end();
        ++label_pointer){

        delete *label_pointer;
    }

    this->record_labels.clear();

    for(lineEdit_pointer = this->record_lineEdits.begin();
        lineEdit_pointer != this->record_lineEdits.end();
        ++lineEdit_pointer){

        delete *lineEdit_pointer;
    }

    this->record_lineEdits.clear();

    for(textEdit_pointer = this->record_textEdits.begin();
        textEdit_pointer != this->record_textEdits.end();
        ++textEdit_pointer){

        delete *textEdit_pointer;
    }

    this->record_textEdits.clear();

    for(button_pointer = this->record_buttons.begin();
        button_pointer != this->record_buttons.end();
        ++button_pointer){

        delete *button_pointer;
    }

    this->record_buttons.clear();

    for(checkBox_pointer = this->record_checkBoxs.begin();
        checkBox_pointer != this->record_checkBoxs.end();
        ++checkBox_pointer){

        delete *checkBox_pointer;
    }

    this->record_checkBoxs.clear();

    for(hLayout_pointer = this->record_hLayouts.begin();
        hLayout_pointer != this->record_hLayouts.end();
        ++hLayout_pointer){

        delete *hLayout_pointer;
    }

    this->record_hLayouts.clear();


    for(vLayout_pointer = this->record_vLayouts.begin();
        vLayout_pointer != this->record_vLayouts.end();
        ++vLayout_pointer){

        delete *vLayout_pointer;
    }

    this->record_vLayouts.clear();

    for(gridLayout_pointer = this->record_gridLayouts.begin();
        gridLayout_pointer != this->record_gridLayouts.end();
        ++gridLayout_pointer){

        delete *gridLayout_pointer;
    }

    this->record_gridLayouts.clear();

    if(this->select_comboBox){
        delete this->select_comboBox;
        this->select_comboBox = NULL;
    }

    if(this->tableWidget){
        delete this->tableWidget;
        this->tableWidget = NULL;
    }

    if(this->main_layout){
        delete this->main_layout;
        this->main_layout = NULL;
    }

}

void TableWindow::set_table(const FileTable &table){
    this->table = table;
}

void TableWindow::update_window(){

    this->clear_window();

    // 头部控件生成
    this->main_layout = new QVBoxLayout(this);
    QHBoxLayout * top_layout = new QHBoxLayout;

    QLabel * table_name_label = new QLabel(this->table.get_tableNameShow());
    this->record_labels.push_back(table_name_label);

    this->select_comboBox = new QComboBox;
    QStringList fields = this->table.get_fieldsShow();
    for(QString field : fields){
        this->select_comboBox->addItem("按 " + field + " 搜索");
    }

    QLineEdit * search_lineEdit = new QLineEdit;
    this->record_lineEdits.push_back(search_lineEdit);

    QPushButton * search_btn = new QPushButton("搜索");
    QPushButton * insert_btn = new QPushButton("增加" + this->table.get_tableNameShow());
    this->record_buttons.push_back(search_btn);
    this->record_buttons.push_back(insert_btn);

    top_layout->addWidget(table_name_label);
    top_layout->addWidget(this->select_comboBox);
    top_layout->addWidget(search_lineEdit);
    top_layout->addWidget(search_btn);
    top_layout->addStretch(1);
    top_layout->addWidget(insert_btn);

    main_layout->addLayout(top_layout);

    // 搜索按钮事件lambda回调

    connect(search_btn, &QPushButton::clicked, [=]{

        int select_index = this->select_comboBox->currentIndex();
        QString key_field = this->table.get_fields().at(select_index);

        this->table.set_key(key_field.trimmed());
        this->table.set_value(search_lineEdit->text().trimmed());

        emit this->signal_sendRequestSearchData(this->table.get_tableName(),
                                                this->table.get_key(),
                                                this->table.get_value());

    });

    // 增加数据按钮事件lambda回调

    connect(insert_btn, &QPushButton::clicked, [=]{

        FileTable new_table = this->table;
        QVector<QStringList> init_datas;
        new_table.set_data(init_datas);

        emit this->signal_sendDataShow(new_table);
    });

    this->record_hLayouts.push_back(top_layout);

    // 初始化table
    this->tableWidget = new QTableWidget;

    this->tableWidget->setColumnCount(this->table.get_colLen() + 2);
    this->tableWidget->setRowCount(this->table.get_rowLen());

    QStringList headers;
    headers << "[选择状态]" << "[操作]" << this->table.get_fieldsShow();
    this->tableWidget->setHorizontalHeaderLabels(headers);

    QVector<QStringList> values = this->table.get_data();
    QVector<QStringList>::iterator values_pointer = values.begin();

    // 渲染数据
    int col_len = this->table.get_colLen() + 2;
    int row_len = this->table.get_rowLen();
    for(int i = 0; i < row_len; ++i){

        for(int j = 0; j < col_len; ++j){

            if(!j){

               QCheckBox * checkbox = new QCheckBox("选择");
               connect(checkbox, &QCheckBox::stateChanged, this, &this->slot_checkChange);
               this->tableWidget->setCellWidget(i, j, checkbox);
               this->record_checkBoxs.push_back(checkbox);

            }else if(j == 1){

                TableButton * table_button = new TableButton();
                table_button->setText("修改");
                table_button->set_id(i);
                connect(table_button, &table_button->signal_sendClick, this, &this->slot_sendDataShow);
                this->record_buttons.push_back(table_button);
                this->tableWidget->setCellWidget(i, j, table_button);

            }else{
                QString str = values_pointer->at(j-2);
                str = str.length() > 20 ? str.mid(0, 20) + "......" : str;
                QLabel * label = new QLabel(str);
                label->setAlignment(Qt::AlignCenter);
                this->tableWidget->setCellWidget(i, j, label);
                this->record_labels.push_back(label);
            }
        }

        ++values_pointer;
    }

    // 调整table 宽高
    QStringList fields_type = this->table.get_fieldsType();
    int column_num = 2;
    for(QString field_type : fields_type){

        if(field_type.compare("int") != 0 && field_type.compare("double") != 0){
            this->tableWidget->setColumnWidth(column_num, 200);
        }
        ++column_num;
    }

    for(int i = 0; i < row_len; ++i){
        this->tableWidget->setRowHeight(i, 40);
    }

    // 底部控件生成
    QVBoxLayout * bottom_layout = new QVBoxLayout;
    this->record_vLayouts.push_back(bottom_layout);

    // 删除按钮生成
    QHBoxLayout * delete_layout = new QHBoxLayout;
    this->record_hLayouts.push_back(delete_layout);

    QPushButton * delete_btn = new QPushButton("删除选中的记录");
    this->record_buttons.push_back(delete_btn);

    QPushButton * all_select_btn = new QPushButton("全选");
    this->record_buttons.push_back(all_select_btn);

    delete_layout->addWidget(all_select_btn);
    delete_layout->addWidget(delete_btn);
    delete_layout->addStretch(1);
    delete_btn->setStyleSheet("QPushButton{"
                              "font-size:20px;"
                              "color:white;"
                              "background:grey;"
                              "font-weight900;"
                              "}");
    all_select_btn->setStyleSheet(delete_btn->styleSheet());

    connect(delete_btn, &QPushButton::clicked, [=]{

        QVector<QStringList> values;
        QVector<QCheckBox *>::iterator check_box_pointer;

        int coord = 0;
        for(check_box_pointer = this->record_checkBoxs.begin();
            check_box_pointer != this->record_checkBoxs.end();
            ++check_box_pointer){

            if((*check_box_pointer)->isChecked()){
                values.push_back(this->table.get_data().at(coord));
            }

            ++coord;
        }

        emit this->signal_sendRequestDelete(this->table.get_tableName(),
                                            this->table.get_fields(),
                                            values);
    });

    // 绑定全选状态信号和槽
    this->all_select_btn = all_select_btn;
    connect(all_select_btn, &QPushButton::clicked, this, &this->slot_allSelectChange);


    bottom_layout->addLayout(delete_layout);

    // 分页按钮生成
    QHBoxLayout * page_layout = new QHBoxLayout;
    this->record_hLayouts.push_back(page_layout);

    int page_sum = this->table.get_sum() / 10;
    page_sum += (this->table.get_sum() % 10) > 0 ? 1 : 0;
    int current_page = this->table.get_pageNum();

    this->create_pageButton(1, "首页", page_layout, false);
    if(current_page > 1){
        this->create_pageButton(current_page - 1, "上一页", page_layout, false);
    }

    // 分页算法
    if(current_page <= 5){

        for(int i = 1; i <= page_sum && i <= 10; ++i){
            this->create_pageButton(i, QString::number(i), page_layout, i == current_page);
        }
    }else{

        for(int i = current_page-5; i <= page_sum && i <= current_page+4; ++i){
            this->create_pageButton(i, QString::number(i), page_layout, i == current_page);
        }
    }


    if(current_page < page_sum){
        this->create_pageButton(current_page + 1, "下一页", page_layout, false);
    }

    bottom_layout->addLayout(page_layout);

    this->main_layout->addWidget(this->tableWidget);
    this->main_layout->addLayout(bottom_layout);
    this->setStyleSheet("QPushButton{"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                        "color:#fff;"
                        "font-size:14px;"
                        "padding:10px;"
                        "}"
                        "QComboBox, QComboBox::focus{"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                        "color:#fff;"
                        "font-size:14px;"
                        "padding:3px;"
                        "}"

                    "QHeaderView::section {"
                       " padding:3px;"
                        "margin:0px;"
                        "color:#F0F0F0;"
                        "border: 1px solid #F0F0F0;"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #636363, stop:1 #575757);"
                   " }"
                        "QLineEdit{"
                        "background:#fff;"
                        "color:black;"
                        "font-size:16px;"
                        "font-weight:700;"
                        "border-radius:6px;"
                        "border: 2px solid #999;"
                        "}"
                        "QTableWidget{"
                        "font-size:16px;"
                        "font-weight:700;"
                        "}");

}

void TableWindow::create_pageButton(const int &id, const QString & text, QHBoxLayout * page_layout, bool is_current){

    TableButton * table_button = new TableButton();
    this->record_buttons.push_back(table_button);
    table_button->setText(text);
    table_button->set_id(id);
    page_layout->addWidget(table_button);
    connect(table_button, &table_button->signal_sendClick, this, &this->slot_pageChange);

    if(is_current){
        table_button->setStyleSheet("QPushButton{"
                                    "color:yellow;"
                                    "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                                    "font-size:30px;"
                                    "font-weight:900;"
                                    "}");
    }else{
        table_button->setStyleSheet("QPushButton{"
                                    "color:white;"
                                    "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                                    "font-size:20px;"
                                    "font-weight:700;"
                                    "}");
    }
}

bool TableWindow::judge_allSelect(){

    QVector<QCheckBox * >::iterator checkBox_pointer;
    for(checkBox_pointer = this->record_checkBoxs.begin();
        checkBox_pointer != this->record_checkBoxs.end();
        ++checkBox_pointer){

        if(!(*checkBox_pointer)->isChecked()){
            return false;
        }

    }

    return true;
}

void TableWindow::slot_allSelectChange(){

    bool flag = true;
    if(this->all_select_btn->text().compare("全选") == 0){
        flag = true;

    }else{
        flag = false;
    }
    QVector<QCheckBox * >::iterator checkBox_pointer;
    for(checkBox_pointer = this->record_checkBoxs.begin();
        checkBox_pointer != this->record_checkBoxs.end();
        ++checkBox_pointer){

        (*checkBox_pointer)->setChecked(flag);

    }

    this->all_select_btn->setText(flag ? "取消全选" : "全选");
}

void TableWindow::slot_pageChange(const int &page_num){

  //  qDebug() << "request page_num";
    emit this->signal_sendRequestPageData(this->table.get_tableName(),
                                          page_num,
                                          this->table.get_key(),
                                          this->table.get_value());
}

void TableWindow::slot_checkChange(const int &status){

    if(status == Qt::Checked && this->judge_allSelect() && this->all_select_btn->text().compare("全选") == 0){
        this->all_select_btn->click();
    }else if(!this->judge_allSelect() && this->all_select_btn->text().compare("取消全选") == 0){
        this->all_select_btn->setText("全选");
    }
}

void TableWindow::slot_sendDataShow(const int &id){

   // qDebug() << "request data show ";
    FileTable new_table = this->table;
    QVector<QStringList> new_data;
    QStringList data = this->table.get_data().at(id);
    new_data.push_back(data);
    new_table.set_data(new_data);

    emit this->signal_sendDataShow(new_table);

}

FileTable TableWindow::get_table()const{
    return this->table;
}
























