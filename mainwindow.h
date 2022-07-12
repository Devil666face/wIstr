#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <includes.h>
#include <animatedlabel.h>
#include <QDoubleSpinBox>

#include "maplayer.h"
#include "settingswindow.h"
#include "equipwindow.h"
#include "plane.h"
#include "airbase.h"
#include "calc.h"
#include "resultwindow.h"

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SettingsWindow* settingswin;
    EquipWindow* equipwin;
    ResultWindow* resultwin;
    void show_result_window();
    calc c;

    //data
    QVector <plane> current_planes;
    QVector <airbase> current_airbases;
    int current_plane_index;
    int current_airbase_index;
    QVector <GeoDataCoordinates> line_atack;
    GeoDataCoordinates atack_point;

    //buttons
    void create_button_equip();
    void create_button_settings();
    void create_button_line();
    void create_button_result();

    //close
    void closeEvent(QCloseEvent* event);

    //MAP
    TestLayer* layer;
    MarbleWidget* map;
    void load_map(QGridLayout *&lay, QString map_theme, int projection);
    void send_data_in_layer(QVector <plane> cur_p, QVector <airbase> cur_a,QVector <GeoDataCoordinates> line_atack);
    void get_line_atack();
    void w_calc();

    //Db
    QSqlDatabase db;
    QSqlQuery *query;
    void open_DB();

    //table
    void create_table_position(QTableWidget *&tableWidget, QVector <plane> current_planes, QVector <airbase> current_airbases);
    void create_table_interface(QTableWidget *&tableWidget);
    void corretc_table_position_visual(QTableWidget *&tableWidget, QFrame *&frame);

    //Dynamic controls
    void create_spinBox_course(int index, QTableWidget *&tableWidget, QString type);
    void create_button_position(int index, QTableWidget *&tableWidget, QString type);

    QVector <GeoDataCoordinates> convert_pair_to_coor(QVector<std::pair<qreal,qreal>> vec_of_pair);

    //flags
    bool all_planes_have_coor(QVector <plane> current_planes);

    //legend
    void draw_legend(QCustomPlot *&widget, QVector <plane> current_planes);



private slots:
    void on_pushButton_equipment();

    void on_pushButton_settings();

    void on_pushButton_result();

    void set_map_settings(QString map_theme, int projection);

    void recive_data(QVector <plane> cur_planes, QVector <airbase> cur_airbases);

    void on_pushButton_coor();

    void mouse_move_on_map(QString string);

    void on_doubleSpinBox_valueChanged(double arg1);

    void get_coordibates_plane(qreal lon,qreal lat,GeoDataCoordinates::Unit);
    void get_coordibates_airbase(qreal lon,qreal lat,GeoDataCoordinates::Unit);
    void get_coordibates_line_atack(qreal lon,qreal lat,GeoDataCoordinates::Unit);

    void zoom_map(int value);

    void get_coor_in_string(QString string);
    void on_pushButton_line();
};
#endif // MAINWINDOW_H
