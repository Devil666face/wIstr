#ifndef PLANE_H
#define PLANE_H
#include <QColor>
#include <QVector>

struct plane{
    QString name;
    qreal tpass;
    qreal tn;
    qreal tr;
    qreal t180;
    qreal tvm;
    qreal Sn;
    qreal Sr;
    qreal Lsm;
    qreal Svm;
    qreal Vc;
    qreal Vgp;

    qreal Sish;
    qreal Dk;
    qreal Ssum;
    qreal tsum;

    qreal Svrvb;

    int alt_type;
    int sfer_type;

    qreal lon;
    qreal lat;
    qreal course;

    QVector <qreal> range_to_airbases;
    bool main_plane;

    QColor color;

    qreal plane_point_range;

    QVector <int> airbase_hit_index;

    bool hit;
};
#endif // PLANE_H
