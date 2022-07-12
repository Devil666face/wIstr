#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QMainWindow>
#include <includes.h>
#include <animatedlabel.h>
#include <QTableView>

namespace Ui {
class DataWindow;
}

class DataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = nullptr);
    ~DataWindow();

private:
    Ui::DataWindow *ui;
    int row_to_remove = -1;

    //buttons
    void create_button_add();
    void create_button_remove();

    //Db
    QSqlDatabase db;
    QSqlTableModel *model;
    void open_DB();
    void open_model(QString table_name, QTableView *&tableView);

    //visual
    void correct_visual(QTableView *&tableView);

    //close
    void closeEvent(QCloseEvent* event);

private slots:
    void on_pushButton_add();
    void on_pushButton_remove();
    void on_tableView_clicked(const QModelIndex &index);

signals:
    void update_window();
};

#endif // DATAWINDOW_H
