#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QObject>
#include <QVector>
#include <QString>

#include "filetable.h"

class Configure : public QObject
{
    Q_OBJECT
public:
    Configure(QObject * parent = 0);

    void set_fileName(const QString & filename);
    void explain_tables();

    QVector<FileTable> get_tables();

private:
    QString filename;
    QVector<FileTable> tables;
};

#endif // CONFIGURE_H
