#ifndef AIRBASE_H
#define AIRBASE_H
struct airbase{
    QString name;
    qreal lon;
    qreal lat;
    qreal course;
    QVector <qreal> Svrvb_vec;

    qreal airbase_point_range;

};

#endif // AIRBASE_H
