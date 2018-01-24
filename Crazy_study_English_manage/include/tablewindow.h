#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QCheckBox>

#include "filetable.h"

class TableWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TableWindow(QWidget *parent = 0);

    void clear_window();
    void set_table(const FileTable & table);
    void update_window();

    void create_pageButton(const int & id, const QString & text,  QHBoxLayout * page_layout, bool is_current = false);
    bool judge_allSelect();


    FileTable get_table()const;


private:
    FileTable table;

    QVector<QLabel *> record_labels;
    QVector<QLineEdit *> record_lineEdits;
    QVector<QTextEdit *> record_textEdits;
    QVector<QPushButton *> record_buttons;
    QVector<QGridLayout *> record_gridLayouts;
    QVector<QVBoxLayout *> record_vLayouts;
    QVector<QHBoxLayout *> record_hLayouts;
    QVector<QCheckBox * > record_checkBoxs;
    QComboBox * select_comboBox;
    QTableWidget * tableWidget;

    QVBoxLayout * main_layout;
    QPushButton * all_select_btn;

signals:
    void signal_sendRequestPageData(
            const QString & table_name,
            const int & page_num,
            const QString & key,
            const QString & value);

    void signal_sendRequestSearchData(
            const QString & table_name,
            const QString & key,
            const QString & value);

    void signal_sendRequestDelete(
            const QString & table_name,
            const QStringList & fields,
            const QVector<QStringList> & values
            );

    void signal_sendDataShow(const FileTable & table);

public slots:

    void slot_allSelectChange();
    void slot_checkChange(const int & status);
    void slot_sendDataShow(const int & id);
    void slot_pageChange(const int & page_num);
};

#endif // TABLEWINDOW_H
