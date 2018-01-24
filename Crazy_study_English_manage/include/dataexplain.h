#ifndef DATAEXPLAIN_H
#define DATAEXPLAIN_H

#include <QObject>
#include <QString>

#include "filetable.h"

class DataExplain : public QObject
{
    Q_OBJECT
public:
    explicit DataExplain(QObject *parent = 0);

    void explain_data(const QString & recv_data);

    void response_error();

    QByteArray transform_login(
            const QString & username,
            const QString & password
            );

    QByteArray transform_requestData(
            const QString & table_name,
            const int & page_num,
            const QString & key,
            const QString & value);

    QByteArray transform_requestData(
            const QString & table_name,
            const QString & key,
            const QString & value);

    QByteArray transform_requestUpdate(
            const QString & table_name,
            const QStringList & fields,
            const QStringList & old_values,
            const QStringList & new_values
            );

    QByteArray transform_requestInsert(
            const QString & table_name,
            const QStringList & fields,
            const QStringList & values);

    QByteArray transform_requestDelete(
            const QString & table_name,
            const QStringList & fields,
            const QVector<QStringList> & values
            );

    QString get_type();
    int get_status();
    FileTable get_table();

private:
    QString type;
    int status;
    FileTable table;
    QString connect_code;

signals:

public slots:
};

#endif // DATAEXPLAIN_H
