#include <QMap>

#include "datashowwindow.h"

DataShowWindow::DataShowWindow(QDialog * parent) : QDialog(parent)
{

    this->main_layout = NULL;
    this->error_dialog = new ErrorDialog();

}

void DataShowWindow::clear_window(){

    QVector<QPushButton *>::iterator buttons_pointer;
    QVector<QLabel *>::iterator labels_pointer;
    QVector<QLineEdit *>::iterator lineEdits_pointer;
    QVector<QTextEdit *>::iterator textEdits_pointer;
    QVector<QHBoxLayout *>::iterator layouts_pointer;

    for(buttons_pointer = this->record_buttons.begin();
        buttons_pointer != this->record_buttons.end();
        ++buttons_pointer){

        delete *buttons_pointer;
    }

    this->record_buttons.clear();

    for(labels_pointer = this->record_labels.begin();
        labels_pointer != this->record_labels.end();
        ++labels_pointer){

        delete *labels_pointer;
    }

    this->record_labels.clear();

    for(lineEdits_pointer = this->record_lineEdits.begin();
        lineEdits_pointer != this->record_lineEdits.end();
        ++lineEdits_pointer){

        delete *lineEdits_pointer;
    }

    this->record_lineEdits.clear();

    for(textEdits_pointer = this->record_textEdits.begin();
        textEdits_pointer != this->record_textEdits.end();
        ++textEdits_pointer){

        delete *textEdits_pointer;
    }

    this->record_textEdits.clear();

    for(layouts_pointer = this->record_hLayouts.begin();
        layouts_pointer != this->record_hLayouts.end();
        ++layouts_pointer){


        delete *layouts_pointer;
    }

    this->record_hLayouts.clear();

    if(this->main_layout){
        delete this->main_layout;
        this->main_layout = NULL;
    }
}

void DataShowWindow::create_window(const FileTable &table){


    this->clear_window();

    this->table = table;

    if(this->table.get_data().length() == 0){
        this->is_insert = true;
    }else{
        this->is_insert = false;
    }

    // 主布局控件
    this->main_layout = new QVBoxLayout(this);

    QHBoxLayout * top_layout = new QHBoxLayout;
    this->record_hLayouts.push_back(top_layout);

    QLabel * label = new QLabel((this->is_insert ? "增加 " : "修改 ") + this->table.get_tableNameShow());
    this->record_labels.push_back(label);
    label->setStyleSheet("QLabel{"
                         "color:white;"
                         "background:grey;"
                         "font-size:20px;"
                         "font-weight:900;"
                         "border-radius:10px;"
                         "padding:10px;"
                         "}");


    top_layout->addWidget(label);
    top_layout->addStretch(1);

    this->main_layout->addLayout(top_layout);

    QStringList fields = this->table.get_fieldsShow();
    QStringList fields_type = this->table.get_fieldsType();
    QStringList limits = this->table.get_fieldsLimit();
    QStringList values;

    QMap<int, QString> record_edit_type;

    if(!this->is_insert){

        values = this->table.get_data().at(0);
    }

    int coord = 0;
    int num = 0;
    for(QString field : fields){

        QHBoxLayout * h_layout = new QHBoxLayout;
        this->record_hLayouts.push_back(h_layout);

        label = new QLabel(field);
        this->record_labels.push_back(label);

        h_layout->addWidget(label);

        QString type = fields_type.at(coord);
        num = 0;

        if(type.startsWith("varchar(")){

            QStringList strs = type.split("varchar(");

            num = strs.at(1).split(")").at(0).toInt();

        }

        QString limit = limits.at(coord);

        if(num <= 100){

            QLineEdit * line_edit = new QLineEdit;
            if(!this->is_insert){
                line_edit->setText(values.at(coord));
            }else if(type.compare("datetime") == 0){
                line_edit->setText("系统自动创建");
            }else if(limit.compare("默认0") == 0){
                line_edit->setText(QString::number(0));
            }else if(this->is_insert && field.endsWith("ID") && !limit.endsWith("外键")){
                line_edit->setText("系统自动创建");
            }

            this->judge_limit(limit, h_layout);

            this->record_lineEdits.push_back(line_edit);
            h_layout->addWidget(line_edit);

            record_edit_type[coord] = "QLineEdit";

        }else {

            QTextEdit * text_edit = new QTextEdit;

            if(!this->is_insert){
                text_edit->setText(values.at(coord));
            }else if(type.compare("datetime") == 0){
                text_edit->setText("系统自动创建");
            }else if(limit.compare("默认0") == 0){
                text_edit->setText(QString::number(0));
            }


            this->judge_limit(limit, h_layout);

            this->record_textEdits.push_back(text_edit);
            h_layout->addWidget(text_edit);
            record_edit_type[coord] = "QTextEdit";
        }

        this->main_layout->addLayout(h_layout);

        ++coord;
    }

    QPushButton * update_btn = new QPushButton(this->is_insert ? "确定增加" : "确定保存");
    this->record_buttons.push_back(update_btn);

    QPushButton * reset_btn = new QPushButton("重置");
    this->record_buttons.push_back(reset_btn);

    QPushButton * close_btn = new QPushButton("关闭");
    this->record_buttons.push_back(close_btn);

    QHBoxLayout * buttons_layout = new QHBoxLayout;
    this->record_hLayouts.push_back(buttons_layout);

    buttons_layout->addWidget(update_btn);
    buttons_layout->addWidget(reset_btn);
    buttons_layout->addWidget(close_btn);
    buttons_layout->addStretch(1);

    this->main_layout->addStretch(1);
    this->main_layout->addLayout(buttons_layout);

    connect(update_btn, &QPushButton::clicked, [=]{

        // 增加数据
        QStringList values = this->get_values(record_edit_type);

        if(!this->judge_edit(values)){

            this->error_dialog->setWindowTitle(this->is_insert ? "Insert error!" : "Update error!");
            this->error_dialog->set_errorMessage("请按要求填写字段数据, 不能留空!");
            this->error_dialog->error_show();
            return ;

        }

        if(this->is_insert){

            emit this->signal_sendRequestInsert(this->table.get_tableName(),
                                                this->table.get_fields(),
                                                values);

        }else{ // 修改数据
            emit this->signal_sendRequestUpdate(this->table.get_tableName(),
                                                this->table.get_fields(),
                                                this->table.get_data().at(0),
                                                values);

        }

    });

    connect(reset_btn, &QPushButton::clicked, [=]{
        this->create_window(this->table);
    });

    connect(close_btn, &QPushButton::clicked, [=]{
        this->close();
    });

    this->setStyleSheet("QPushButton{"
                        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);"
                        "color:#fff;"
                        "font-size:14px;"
                        "padding:10px;"
                        "}"
                        );

    if(this->is_insert){

        this->setWindowTitle("增加 " + this->table.get_tableNameShow());
    }else{

        this->setWindowTitle("修改 " + this->table.get_tableNameShow());
    }

}

QStringList DataShowWindow::get_values(const QMap<int, QString> & map_edit_type){

    QStringList values;
    QVector<QLineEdit *>::iterator lineEdit_pointer = this->record_lineEdits.begin();
    QVector<QTextEdit *>::iterator textEdit_pointer = this->record_textEdits.begin();

    int fields_len = this->table.get_fields().length();

    for(int i = 0; i < fields_len; ++i){

        QString edit_type = map_edit_type[i];

        if(edit_type.compare("QLineEdit") == 0 && lineEdit_pointer != this->record_lineEdits.end()){

            values << (*lineEdit_pointer)->text();
            ++lineEdit_pointer;

        }else if(edit_type.compare("QTextEdit") == 0 && textEdit_pointer != this->record_textEdits.end()){

            values << (*textEdit_pointer)->toPlainText();
            ++textEdit_pointer;

        }else{
            QStringList err_values;
            return err_values;
        }

    }

    return values;
}

bool DataShowWindow::judge_edit(const QStringList &values){

    QStringList limits = this->table.get_fieldsLimit();
    int coord = 0;
    for(QString value : values){
        QString limit = limits.at(coord);
        if(value.isEmpty() && (limit.compare("否") == 0 || limit.compare("否外键") == 0)){
            return false;
        }

        ++coord;
    }

    return true;
}

void DataShowWindow::judge_limit(const QString &limit, QHBoxLayout *layout){

    if(limit.compare("否") == 0 || limit.compare("否外键") == 0){
        QLabel * label = new QLabel("*");
        label->setStyleSheet("color:red;");
        this->record_labels.push_back(label);
        layout->addWidget(label);
    }
}
























