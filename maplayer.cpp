#include "maplayer.h"
#include "mainwindow.h"

TestLayer::TestLayer():LayerInterface()
{

}

bool TestLayer::render(GeoPainter* painter, ViewportParams* viewport,
    const QString& /*renderPos*/, GeoSceneLayer* /*layer*/)
{
    if (l_atack.size()==4)
    {
        draw_line_atack(painter,l_atack,QColor(255,0,0,50));
    }

    if (cur_planes.size()!=0)
    {
        int number = 1;
        foreach (auto p, cur_planes)
        {
            if ((p.lon!=0)&&(p.lat!=0))
            {
                GeoDataCoordinates center(p.lon,p.lat,GeoDataCoordinates::Radian);
                painter->drawImage(center,rotate_image(p.course,
                                                       get_zoom_image_plane(get_zoom_interval(zoom_value,
                                                                                              zoom_vector))));
                if (p.hit)
                    painter->drawImage(center,rotate_image(p.course,
                                                           get_zoom_image_cross(get_zoom_interval(zoom_value,
                                                                                                  zoom_vector))));
                painter->setPen(QColor(Qt::yellow));
                painter->drawText(center,QString("%1").arg(number));
            }
            number++;
        }
    }

    if (cur_airbases.size()!=0)
    {
        foreach (auto a, cur_airbases)
        {
            if ((a.lon!=0)&&(a.lat!=0))
            {
                GeoDataCoordinates center(a.lon,a.lat,GeoDataCoordinates::Radian);
                painter->drawImage(center,rotate_image(a.course,
                                                       get_zoom_image_airbase(get_zoom_interval(zoom_value,
                                                                                              zoom_vector))));
                painter->setPen(QColor(Qt::black));
                painter->drawText(center,a.name,get_sdvig_text(a.name));

                foreach (auto r, a.Svrvb_vec)
                {
                    draw_ring(painter,r,center,cur_planes.value(a.Svrvb_vec.indexOf(r)).color);
                }
            }
        }
    }



    return true;
}

QStringList TestLayer::renderPosition() const
{
    return QStringList() << "USER_TOOLS";
}

QVector<qreal> TestLayer::get_zoom_vector(int max_zoom, int min_zoom)
{
    qreal zoom_step = (max_zoom-min_zoom)/7;
    QVector <qreal> zoom_vector;
    zoom_vector.append(min_zoom);
    for (int i=1;i<8;i++)
        zoom_vector.append(min_zoom+zoom_step*i);
    return zoom_vector;
}

int TestLayer::get_zoom_interval(int zoom_value, QVector<qreal> zoom_vector)
{
    for (int i=0;i<zoom_vector.size();i++)
    {
        if (i==zoom_vector.size()-1) return i;
        if ((zoom_vector[i]<=zoom_value)&&(zoom_value<zoom_vector[i+1])) return i;
    }
}

QImage TestLayer::get_zoom_image_plane(int interval)
{
    switch (interval) {
    case 0:
        return QImage(":/new/prefix1/build/images/plane_32.png");
        break;
    case 1:
        return QImage(":/new/prefix1/build/images/plane_48.png");
        break;
    case 2:
        return QImage(":/new/prefix1/build/images/plane_64.png");
        break;
    case 3:
        return QImage(":/new/prefix1/build/images/plane_72.png");
        break;
    case 4:
        return QImage(":/new/prefix1/build/images/plane_96.png");
        break;
    case 5:
        return QImage(":/new/prefix1/build/images/plane_128.png");
        break;
    case 6:
        return QImage(":/new/prefix1/build/images/plane_256.png");
        break;
    case 7:
        return QImage(":/new/prefix1/build/images/plane_512.png");
        break;
    default:
        break;
    }
}

QImage TestLayer::get_zoom_image_cross(int interval)
{
    switch (interval) {
    case 0:
        return QImage(":/new/prefix1/build/images/cross_32.png");
        break;
    case 1:
        return QImage(":/new/prefix1/build/images/cross_48.png");
        break;
    case 2:
        return QImage(":/new/prefix1/build/images/cross_64.png");
        break;
    case 3:
        return QImage(":/new/prefix1/build/images/cross_72.png");
        break;
    case 4:
        return QImage(":/new/prefix1/build/images/cross_96.png");
        break;
    case 5:
        return QImage(":/new/prefix1/build/images/cross_128.png");
        break;
    case 6:
        return QImage(":/new/prefix1/build/images/cross_256.png");
        break;
    case 7:
        return QImage(":/new/prefix1/build/images/cross_512.png");
        break;
    default:
        break;
    }
}

QImage TestLayer::get_zoom_image_airbase(int interval)
{
    switch (interval) {
    case 0:
        return QImage(":/new/prefix1/build/images/airbase_32.png");
        break;
    case 1:
        return QImage(":/new/prefix1/build/images/airbase_48.png");
        break;
    case 2:
        return QImage(":/new/prefix1/build/images/airbase_64.png");
        break;
    case 3:
        return QImage(":/new/prefix1/build/images/airbase_72.png");
        break;
    case 4:
        return QImage(":/new/prefix1/build/images/airbase_96.png");
        break;
    case 5:
        return QImage(":/new/prefix1/build/images/airbase_128.png");
        break;
    case 6:
        return QImage(":/new/prefix1/build/images/airbase_256.png");
        break;
    case 7:
        return QImage(":/new/prefix1/build/images/airbase_512.png");
        break;
    default:
        break;
    }
}

QImage TestLayer::rotate_image(qreal course, QImage image)
{
    QImage image_return;
    QImage image_for_return = image;
    QTransform transform;
    transform.rotate(course);
    image_return = image.transformed(transform);
    return image_return;

}

int TestLayer::get_sdvig_text(QString name)
{
    int size = name.size();
    return -size*3;
}

void TestLayer::draw_ring(GeoPainter *painter, qreal radius, GeoDataCoordinates center, QColor color)
{
    GeoDataLinearRing ring;
    for (qreal a=0.0;a<=360.0;a+=1.0) ring.append(center.moveByBearing(a/57.3,(radius)/6371));
    painter->setPen(QPen(color,2));
    painter->setBrush(QBrush(QColor(0,0,0,0)));
    painter->drawPolygon(ring);
}

void TestLayer::draw_line_atack(GeoPainter *painter, QVector<GeoDataCoordinates> l_atack, QColor color)
{
    GeoDataLinearRing polygon_atack;
    polygon_atack.append(l_atack.value(0));
    polygon_atack.append(l_atack.value(1));
    polygon_atack.append(l_atack.value(3));
    polygon_atack.append(l_atack.value(2));
    painter->setPen(QPen(QColor(0,0,0,0)));
    painter->setBrush(QBrush(color));
    //painter->setBrush(QBrush(color,Qt::DiagCrossPattern));
    painter->drawPolygon(polygon_atack);
}

QColor TestLayer::get_random_color()
{
    int r = Random::get(0,255);
    int g = Random::get(0,255);
    int b = Random::get(0,255);
    return QColor(r,g,b);
}

TestLayer::~TestLayer()
{
}


