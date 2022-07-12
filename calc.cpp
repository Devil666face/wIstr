#include "calc.h"

calc::calc()
{

}

QVector<plane> calc::count_all_ranges_plane_airbase(QVector<plane> cur_planes, QVector<airbase> cur_airbases)
{
    for (int i=0;i<cur_planes.size();i++)
    {
        cur_planes[i].range_to_airbases.clear();
        foreach (auto one_airbase, cur_airbases)
        {
            cur_planes[i].range_to_airbases.append(get_range(cur_planes[i],one_airbase));
        }
    }
    return get_main_plane(cur_planes);
}

QVector<plane> calc::get_main_plane(QVector<plane> cur_planes)
{
    QVector <qreal> avg_ranges;
    foreach (auto p, cur_planes)
    {
        qreal sum = 0;
        foreach (auto range, p.range_to_airbases)
        {
            sum+=range;
            qDebug()<<"Дальность"<<range;
        }
        avg_ranges.append(sum/p.range_to_airbases.size());
    }
    int max_index = get_index_of_max(avg_ranges);
    qDebug()<<max_index;
    for (int i=0;i<cur_planes.size();i++)
    {
        if (i==max_index) cur_planes[i].main_plane=true;
        else cur_planes[i].main_plane=false;
    }
    return cur_planes;
}

int calc::get_index_of_max(QVector<qreal> vec)
{
    qreal max = vec.first();
    int index_of_max = 0;
    for (int i=0;i<vec.size();i++)
    {
        if (vec[i]>max)
        {
            max=vec[i];
            index_of_max = i;
        }
    }
    return index_of_max;
}

qreal calc::get_width_line_atack(QVector<plane> cur_planes)
{
    QVector <qreal> temp;
    foreach (auto one_plane, cur_planes)
    {
        foreach (auto p, cur_planes)
        {
            temp.append(get_range(one_plane,p));
        }
    }
    qreal sum = 0;
    temp = remove_duplicates(temp);
    foreach (auto val, temp)
    {
        sum+=val;
        qDebug()<<"Расстояние между самолетами"<<val;
    }
    qDebug()<<"Ширина полосы налета"<<sum/cur_planes.size();
    return sum/temp.size();
}

QVector<qreal> calc::remove_duplicates(QVector<qreal> vec)
{
    QStringList vec_str;
    foreach (auto value, vec)
        vec_str.append(QString("%1").arg(value));
    vec_str.removeDuplicates();
    QVector <qreal> vec_for_return;
    foreach (auto string, vec_str)
    {
        if (string.toDouble()>1)
            vec_for_return.append(string.toDouble());
    }
    return vec_for_return;
}

qreal calc::get_bearning(qreal lon_start, qreal lat_start, qreal lon_finish, qreal lat_finish)
{
    qreal y = qSin(lon_finish-lon_start) * qCos(lat_finish);
    qreal x = qCos(lat_start)*qSin(lat_finish) -
              qSin(lat_start)*qCos(lat_finish)*qCos(lon_finish-lon_start);
    qreal bearning = qAtan2(y, x);
    return bearning;
    //return bearning*180/3.14159265;
}

qreal calc::get_bearning_for_planes(qreal lon_start, qreal lat_start, qreal lon_finish, qreal lat_finish)
{
    qreal y = qSin(lon_finish-lon_start) * qCos(lat_finish);
    qreal x = qCos(lat_start)*qSin(lat_finish) -
              qSin(lat_start)*qCos(lat_finish)*qCos(lon_finish-lon_start);
    qreal bearning = qAtan2(y, x)*180/3.14159265;
    if (bearning<0) return 180+(180+bearning);
    else return bearning;
}

QVector<std::pair<qreal, qreal> > calc::get_line_atack_coordinates(qreal bearning, qreal width, qreal lon_start, qreal lat_start, qreal lon_finish, qreal lat_finish)
{
    QVector <std::pair<qreal,qreal>> for_return;
    for_return.append(get_one_coor_for_line(bearning+1.570796327,lon_start,lat_start,width));
    for_return.append(get_one_coor_for_line(bearning-1.570796327,lon_start,lat_start,width));
    for_return.append(get_one_coor_for_line(bearning+1.570796327,lon_finish,lat_finish,width));
    for_return.append(get_one_coor_for_line(bearning-1.570796327,lon_finish,lat_finish,width));
    return for_return;
}

std::pair<qreal, qreal> calc::get_one_coor_for_line(qreal bearning, qreal lon, qreal lat, qreal width)
{
    qreal radius = 6371;
    qreal lat_finish = qAsin( qSin(lat)*qCos(width/radius) +
                          qCos(lat)*qSin(width/radius)*qCos(bearning) );
    qreal lon_finish = lon + qAtan2(qSin(bearning)*qSin(width/radius)*qCos(lat),
                               qCos(width/radius)-qSin(lat)*qSin(lat_finish));
    std::pair<qreal,qreal> coor;
    coor.first = lon_finish;
    coor.second = lat_finish;
    return coor;
}

QVector<plane> calc::update_all_plane_course(QVector<plane> cur_planes, qreal lon_finish, qreal lat_finish)
{
    for (int i=0;i<cur_planes.size();i++)
    {
        cur_planes[i].course = get_bearning_for_planes(cur_planes[i].lon,cur_planes[i].lat,lon_finish,lat_finish);
        qDebug()<<"Новый курс самолета"<<cur_planes[i].course;
    }
    return cur_planes;
}


qreal calc::get_range(plane p, airbase a)
{
    qreal rad90 = 1.570796327;
    qreal Earth_rad = 6371;
    qreal range = Earth_rad*(qAcos(qCos((rad90-p.lat))*qCos((rad90-a.lat))+qSin((rad90-p.lat))*qSin((rad90-a.lat))*qCos((a.lon-p.lon))));
    return range;
}

qreal calc::get_range(plane p, plane p_sec)
{
    qreal rad90 = 1.570796327;
    qreal Earth_rad = 6371;
    qreal range = Earth_rad*(qAcos(qCos((rad90-p.lat))*qCos((rad90-p_sec.lat))+qSin((rad90-p.lat))*qSin((rad90-p_sec.lat))*qCos((p_sec.lon-p.lon))));
    return range;
}

qreal calc::get_range(plane p, qreal lon, qreal lat)
{
    qreal rad90 = 1.570796327;
    qreal Earth_rad = 6371;
    qreal range = Earth_rad*(qAcos(qCos((rad90-p.lat))*qCos((rad90-lat))+qSin((rad90-p.lat))*qSin((rad90-lat))*qCos((lon-p.lon))));
    return range;
}

qreal calc::get_range(airbase a, qreal lon, qreal lat)
{
    qreal rad90 = 1.570796327;
    qreal Earth_rad = 6371;
    qreal range = Earth_rad*(qAcos(qCos((rad90-a.lat))*qCos((rad90-lat))+qSin((rad90-a.lat))*qSin((rad90-lat))*qCos((lon-a.lon))));
    return range;
}

qreal calc::get_hit_on_radius(qreal plane_point_range, qreal airbase_point_range, qreal plane_airbase_range, qreal radius)
{
    qreal alfa = (qAcos((pow(airbase_point_range,2)+pow(plane_point_range,2)-pow(plane_airbase_range,2))/(2*airbase_point_range*plane_point_range)));
    qreal gamma = qAsin((airbase_point_range*qSin(alfa))/(radius));
    qreal beta = (3.14159265)-alfa-gamma;
    qreal d_fake = (radius*qSin(beta))/(qSin(alfa));
    return (plane_point_range - d_fake);
}

plane calc::create_current_plane(plane data, QString name, int alt_type, int sfer_type, qreal V, qreal S, qreal D)
{
    plane p;
    p.name = name;
    p.tsum = get_tsum(data,alt_type,sfer_type);
    p.Ssum = get_ssum(data,alt_type,sfer_type);
    p.Vc = V;
    p.Sish = S;
    p.Dk = get_D(sfer_type,D);
    p.alt_type = alt_type;
    p.sfer_type = sfer_type;
    p.Vgp = data.Vgp;

    p.lon = 0;
    p.lat = 0;
    p.course = 0;

    p.Svrvb = get_Svrvb(p);

    p.color = get_random_color();

    p.main_plane = false;

    p.hit = false;

    return p;
}

qreal calc::get_tsum(plane data, int alt_type, int sfer_type)
{
    if (sfer_type==0)
    {
        switch (alt_type) {
        case 0:
            return data.tpass+data.tn+data.tr+data.t180+data.tvm;
            break;
        case 1:
            return data.tpass+data.tn+data.t180;
            break;
        case 2:
            return data.tpass+data.tn+data.tvm+data.t180;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (alt_type) {
        case 0:
            return data.tpass+data.tn+data.tr+data.tvm;
            break;
        case 1:
            return data.tpass+data.tn;
            break;
        case 2:
            return data.tpass+data.tn+data.tvm;
            break;
        default:
            break;
        }
    }
}

qreal calc::get_ssum(plane data, int alt_type, int sfer_type)
{
    if (sfer_type==0)
    {
        switch (alt_type) {
        case 0:
            return data.Sn+data.Sr-data.Lsm-data.Svm;
            break;
        case 1:
            return data.Sn;
            break;
        case 2:
            return data.Sn+data.Svm;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (alt_type) {
        case 0:
            return data.Sn+data.Sr+data.Svm;
            break;
        case 1:
            return data.Sn;
            break;
        case 2:
            return data.Sn+data.Svm;
            break;
        default:
            break;
        }
    }
}

qreal calc::get_D(int sfer_type, qreal D)
{
    switch (sfer_type) {
    case 0:
        return -D;
        break;
    case 1:
        return D;
        break;
    default:
        break;
    }
}

qreal calc::get_Svrvb(plane p)
{
    qreal Svrvb = (p.Sish-p.Vc/100*p.tsum+(p.Vc/p.Vgp)*(p.Ssum+p.Dk))/((p.Vc/p.Vgp)+1);
    qDebug()<<"Sврвб"<<Svrvb<<"Sб"<<Svrvb-p.Ssum+p.Dk;
    if ((Svrvb-p.Ssum+p.Dk)>0)
    {
        qDebug()<<"Расчет по первой формуле";
        return Svrvb;
    }
    else
    {
        qDebug()<<"Расчет по второй формуле";
        return p.Sish-p.Vc/100*p.tsum;
    }
}

QColor calc::get_random_color()
{
    int r = Random::get(0,255);
    int g = Random::get(0,255);
    int b = Random::get(0,255);
    return QColor(r,g,b);
}
