#ifndef CALC_H
#define CALC_H
#include <QString>
#include <plane.h>
#include <airbase.h>
#include <QDebug>
#include "random.h"
#include <QColor>
#include <QtMath>
#include <includes.h>

class calc
{
public:
    calc();
    QVector <plane> count_all_ranges_plane_airbase(QVector <plane> cur_planes,QVector <airbase> cur_airbases);
    QVector <plane> get_main_plane(QVector <plane> cur_planes);
    int get_index_of_max(QVector <qreal> vec);

    qreal get_width_line_atack(QVector <plane> cur_planes);
    QVector <qreal> remove_duplicates(QVector <qreal> vec);
    qreal get_bearning(qreal lon_start, qreal lat_start, qreal lon_finish, qreal lat_finish);
    qreal get_bearning_for_planes(qreal lon_start, qreal lat_start, qreal lon_finish, qreal lat_finish);
    QVector <std::pair<qreal,qreal>> get_line_atack_coordinates(qreal bearning, qreal width, qreal lon_start,
                                                                                             qreal lat_start,
                                                                                             qreal lon_finish,
                                                                                             qreal lat_finish);
    std::pair<qreal,qreal> get_one_coor_for_line(qreal bearning, qreal lon, qreal lat, qreal width);

    QVector <plane> update_all_plane_course(QVector <plane> cur_planes, qreal lon_finish, qreal lat_finish);

    qreal get_range(plane p, airbase a);
    qreal get_range(plane p, plane p_sec);
    qreal get_range(plane p, qreal lon, qreal lat);
    qreal get_range(airbase a, qreal lon, qreal lat);
    qreal get_hit_on_radius(qreal plane_point_range, qreal airbase_point_range, qreal plane_airbase_range, qreal radius);

    plane create_current_plane(plane data, QString name, int alt_type, int sfer_type, qreal V, qreal S, qreal D);
    qreal get_tsum(plane data,int alt_type, int sfer_type);
    qreal get_ssum(plane data,int alt_type, int sfer_type);
    qreal get_D(int sfer_type, qreal D);
    qreal get_Svrvb(plane p);
    QColor get_random_color();
};

#endif // CALC_H
