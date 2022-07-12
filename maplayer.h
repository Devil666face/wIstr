#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "includes.h"
#include "plane.h"
#include "airbase.h"
#include "random.h"

using namespace Marble;

class TestLayer : public LayerInterface
{
    virtual bool render(GeoPainter* painter, ViewportParams* viewport,
                        const QString& renderPos = "NONE", GeoSceneLayer* layer = nullptr)override;
    virtual QStringList renderPosition() const override;


public:
    TestLayer();
   ~TestLayer()override;

    int maximum_zoom;
    int minimum_zoom;
    int zoom_value;
    QVector <qreal> zoom_vector;
    QVector <qreal> get_zoom_vector(int max_zoom, int min_zoom);

    QVector <plane> cur_planes;
    QVector <airbase> cur_airbases;
    QVector <GeoDataCoordinates> l_atack;

private:
    int get_zoom_interval(int zoom_value, QVector<qreal> zoom_vector);
    QImage get_zoom_image_plane(int interval);
    QImage get_zoom_image_cross(int interval);
    QImage get_zoom_image_airbase(int interval);
    QImage rotate_image(qreal course, QImage image);
    int get_sdvig_text(QString name);
    void draw_ring(GeoPainter *painter, qreal radius, GeoDataCoordinates center, QColor color);
    void draw_line_atack(GeoPainter *painter, QVector <GeoDataCoordinates> l_atack, QColor color);
    QColor get_random_color();


};

#endif // MAPLAYER_H
