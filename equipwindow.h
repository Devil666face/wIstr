#ifndef EQUIPWINDOW_H
#define EQUIPWINDOW_H

#include <QMainWindow>
#include <includes.h>
#include <animatedlabel.h>
#include <QListWidgetItem>
#include <QTableWidget>

#include "datawindow.h"
#include "plane.h"
#include "calc.h"
#include "airbase.h"

namespace Ui {
class EquipWindow;
}

class EquipWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EquipWindow(QWidget *parent = nullptr);
    ~EquipWindow();

private:
    Ui::EquipWindow *ui;
    DataWindow* datawin;
    QString currentName;

    calc c;

    //data
    QVector <plane> data_of_plane;
    QVector <plane> current_planes;
    QVector <airbase> current_airbases;

    QVector <airbase> create_current_airbases(QTableWidget *&tableWidget, QVector <plane> cur_planes);

    //buttons
    void create_button_data();
    void create_button_add();

    //listWidget
    void add_item_in_data_listWidget(QVector <plane> data, QListWidget *&listWidget);
    void insert_data_in_parms_on_click(QString name, QVector <plane> data);

    //table
    void create_table_current(QTableWidget *&tableWidget);
    void correct_visual(QTableWidget *&tableWidget);
    void update_table(QTableWidget *&tableWidget, QVector<plane>data);
    void create_table_airbase(QTableWidget *&tableWidget);

    //Db
    QSqlDatabase db;
    QSqlQuery *query;
    QVector <plane> load_DB(QString table_name);
    void open_DB();

    int index_of_plane(QString name, QVector <plane> data);

    //close
    void closeEvent(QCloseEvent* event);



private slots:
    void update_slot();
    void on_pushButton_data();

    void on_pushButton_add();
    void on_tableWidget_current_cellDoubleClicked(int row, int column);
    void on_tableWidget_current_cellClicked(int row, int column);
    void on_listWidget_data_currentTextChanged(const QString &currentText);

    void on_spinBox_airbase_valueChanged(int arg1);

signals:
    void send_data(QVector <plane> current_planes, QVector <airbase> current_airbases);
};

#endif // EQUIPWINDOW_H
