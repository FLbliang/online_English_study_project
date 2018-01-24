#include <QRegExp>
#include <QTextStream>
#include <QFile>
#include "configure.h"

#include "filetable.h"


Configure::Configure(QObject * parent) : QObject(parent)
{

}

void Configure::set_fileName(const QString &filename){
    this->filename = filename;
}

void Configure::explain_tables(){

    QFile file(this->filename);

    if(file.open(QFile::ReadOnly | QFile::Text)){

        QTextStream stream(&file);
        QString line;
        FileTable table;
        QStringList list;
        QStringList fields;
        QStringList fields_type;
        QStringList fields_show;
        QStringList fields_limit;
        do{

            line = stream.readLine();
            list = line.split(QRegExp("[\t]+"));
            line = list.at(0);
            if(line.isEmpty() || line.compare("$end") == 0){
                break;
            }else if(line.compare("$next") == 0){
               table.set_fields(fields);
               table.set_fieldsType(fields_type);
               table.set_fieldsShow(fields_show);
               table.set_fields_limit(fields_limit);

               this->tables.push_back(table);
               table.clear();
               fields.clear();
               fields_type.clear();
               fields_show.clear();
               fields_limit.clear();

            }else{

                if(list.length() < 4){
                    this->tables.clear();
                    break;
                }

                if(table.get_tableName().compare("") == 0){

                    table.set_tableNameShow(list.at(0));
                    table.set_tableName(list.at(1));
                    table.set_manageTypeShow(list.at(2));
                    table.set_manageType(list.at(3));
                    continue;
                }

                fields_show << list.at(0);
                fields << list.at(1);
                fields_type << list.at(2);
                fields_limit << list.at(3);
            }



        }while(true);

    }
}

QVector<FileTable> Configure::get_tables(){

    return this->tables;

}
