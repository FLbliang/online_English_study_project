#ifndef DATASHOWWINDOW_H
#define DATASHOWWINDOW_H

#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "errordialog.h"
#include "filetable.h"

class DataShowWindow : public QDialog
{
    Q_OBJECT
public:
    DataShowWindow(QDialog * parent = 0);

    QStringList get_values(const QMap<int, QString> & map_edit_type);
    bool judge_edit(const QStringList & values);
    void create_window(const FileTable & table);
    void judge_limit(const QString & limit, QHBoxLayout * layout);
    void clear_window();

private:
    FileTable table;
    QVector<QPushButton *> record_buttons;
    QVector<QLabel *> record_labels;
    QVector<QLineEdit *> record_lineEdits;
    QVector<QTextEdit *> record_textEdits;
    QVector<QHBoxLayout *> record_hLayouts;

    QVBoxLayout * main_layout;
    ErrorDialog * error_dialog;

    bool is_insert;

signals:
    void signal_sendRequestUpdate(
            const QString & table_name,
            const QStringList & fields,
            const QStringList & old_values,
            const QStringList & new_values
            );

    void signal_sendRequestInsert(
            const QString & table_name,
            const QStringList & fields,
            const QStringList & values);

};

#endif // DATASHOWWINDOW_H
