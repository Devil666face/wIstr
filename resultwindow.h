#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>

#include "plane.h"
#include "airbase.h"
#include "qcustomplot.h"
#include "random.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QMainWindow
{
    Q_OBJECT

public:
    ResultWindow(QWidget *parent, QVector <plane> current_planes, QVector <airbase> current_airbase);
    ~ResultWindow();

private:
    Ui::ResultWindow *ui;
    void create_destroy_widget(QHBoxLayout *&lay, QVector <plane> current_planes, QVector <airbase> current_airbase);
    void correct_visual(QTableWidget *&tableWidget);
    QTableWidget* create_result_table(QVector <plane> current_planes);
    QTableWidget* create_airbase_table(QVector <plane> current_planes, QVector <airbase> current_airbase, int j);
    int get_count_destroy_planes(QVector <plane> current_planes);
    int get_row_height(QTableWidget *&tableWidget);
    qreal get_w(int destroy, int all);
    QVector <int> row_heights;
    void update_frame_height(QVector <int> row_heights, QFrame *&frame);
    void draw_graph(QCustomPlot *&widget, QVector <plane> current_planes, QVector <airbase> current_airbase);
    QVector <int> get_hit_counts(QVector <plane> current_planes, QVector <airbase> current_airbase);
    int get_max_el(QVector <int> vector);
    QVector <int> get_x_vector(QVector <airbase> current_airbase);
    QStringList get_airbase_names(QVector <airbase> current_airbase);


};

#endif // RESULTWINDOW_H
