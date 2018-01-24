#ifndef FILETABLE_H
#define FILETABLE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

class FileTable
{

public:

    FileTable();

    void clear();

    void set_manageType(const QString & manage_type);
    void set_manageTypeShow(const QString & manage_type_show);
    void set_tableName(const QString & table_name);
    void set_tableNameShow(const QString & table_name_show);
    void set_fields(const QStringList & fields);
    void set_fieldsType(const QStringList & fields_type);
    void set_fieldsShow(const QStringList & fields_show);
    void set_fields_limit(const QStringList & fields_limit);
    void set_data(const QVector<QStringList> & data);
    void set_pageNum(const int & page_num);
    void set_sum(const int & sum);
    void set_rowLen(const int & row_len);
    void set_colLen(const int & col_len);

    void set_key(const QString & key);
    void set_value(const QString & value);
    QString get_key()const;
    QString get_value()const;

    QString get_manageType() const;
    QString get_manageTypeShow() const;
    QString get_tableName() const;
    QString get_tableNameShow() const;
    QStringList get_fields() const;
    QStringList get_fieldsType() const;
    QStringList get_fieldsShow() const;
    QStringList get_fieldsLimit() const;
    QVector<QStringList> get_data() const;
    int get_pageNum() const;
    int get_sum() const;
    int get_rowLen() const;
    int get_colLen() const;

private:
    QString manage_type;
    QString manage_type_show;
    QString table_name;
    QString table_name_show;
    QStringList fields;
    QStringList fields_type;
    QStringList fields_show;
    QStringList fields_limit;
    QVector<QStringList> data;

    QString key;
    QString value;

    int page_num;
    int sum;
    int row_len;
    int col_len;

signals:

public slots:
};

#endif // TABLE_H
