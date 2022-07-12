#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //visual on start
    ui->frame_table->hide();
    ui->widget_legend->xAxis->setVisible(false);
    ui->widget_legend->yAxis->setVisible(false);
    ui->widget_legend->hide();

    //buttons
    create_button_equip();
    create_button_result();
    create_button_settings();
    create_button_line();

    //map
    load_map(ui->map_lay,"earth/plain/plain.dgml",0);

    //Db
    open_DB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_result_window()
{
    resultwin = new ResultWindow(this,current_planes,current_airbases);
    resultwin->setWindowModality(Qt::WindowModal);
    resultwin->show();
}

void MainWindow::create_button_equip()
{
    AnimatedLabel* equipmentButton = new AnimatedLabel(this,":/new/prefix1/icons/equip.gif","Нажмите для создания оснащения",64,64);
    ui->mainToolBar->addWidget(equipmentButton);
    connect(equipmentButton,SIGNAL(clicked()),SLOT(on_pushButton_equipment()));
}

void MainWindow::create_button_settings()
{
    AnimatedLabel* settingsButton = new AnimatedLabel(this,":/new/prefix1/icons/settings.gif","Нажмите управления настройками карты",64,64);
    ui->mainToolBar->addWidget(settingsButton);
    connect(settingsButton,SIGNAL(clicked()),SLOT(on_pushButton_settings()));
}

void MainWindow::create_button_line()
{
    AnimatedLabel* lineButton = new AnimatedLabel(this,":/new/prefix1/icons/line.gif","Нажмите для задания полосы налета",64,64);
    ui->lay_but_line->addWidget(lineButton);
    connect(lineButton,SIGNAL(clicked()),SLOT(on_pushButton_line()));
}

void MainWindow::create_button_result()
{
    AnimatedLabel* resultButton = new AnimatedLabel(this,":/new/prefix1/icons/result.gif","Нажмите для показа резульатов моделирования",64,64);
    ui->mainToolBar->addWidget(resultButton);
    connect(resultButton,SIGNAL(clicked()),SLOT(on_pushButton_result()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    db.close();
    delete map;
    delete this;
}

void MainWindow::load_map(QGridLayout *&lay, QString map_theme, int projection)
{
    map=new MarbleWidget();
    map->setMapThemeId(map_theme);
    map->setShowCrosshairs(false);
    foreach(AbstractFloatItem *floatItem, map->floatItems())
        floatItem->hide();
    foreach(RenderPlugin* plugin, map->renderPlugins())
        if(plugin->nameId() == "stars") plugin->setEnabled(false);
    map->setShowCities(true);

    layer = new TestLayer();
    map->addLayer(layer);
    lay->addWidget(map);

    GeoDataCoordinates Moscow(37.61/57.3,55.75/57.3,GeoDataCoordinates::Radian);
    map->centerOn(Moscow);
    map->setProjection(projection);

    connect(map,SIGNAL(mouseMoveGeoPosition(QString)),SLOT(mouse_move_on_map(QString)));
    connect(map,SIGNAL(zoomChanged(int)),this,SLOT(zoom_map(int)));

    map->zoomView(1500);

    layer->maximum_zoom=map->maximumZoom();
    layer->minimum_zoom=map->minimumZoom();
    layer->zoom_vector=layer->get_zoom_vector(layer->maximum_zoom,layer->minimum_zoom);
    map->update();
}

void MainWindow::send_data_in_layer(QVector<plane> cur_p, QVector<airbase> cur_a, QVector<GeoDataCoordinates> line_atack)
{
    layer->cur_planes=cur_p;
    layer->cur_airbases=cur_a;
    layer->l_atack = line_atack;
    map->update();
}

void MainWindow::get_line_atack()
{
    qreal bearning = c.get_bearning(line_atack.first().longitude(),line_atack.first().latitude(),line_atack.last().longitude(),line_atack.last().latitude());
    line_atack = convert_pair_to_coor(c.get_line_atack_coordinates(bearning,
                                                                   c.get_width_line_atack(current_planes),
                                                                   line_atack.first().longitude(),
                                                                   line_atack.first().latitude(),
                                                                   line_atack.last().longitude(),
                                                                   line_atack.last().latitude()));
    send_data_in_layer(current_planes,current_airbases,line_atack);
}

void MainWindow::w_calc()
{
    if (line_atack.size()==4)
    {
        for (int i=0;i<current_planes.size();i++)
        {
            current_planes[i].hit = false;
            current_planes[i].plane_point_range = c.get_range(current_planes[i],atack_point.longitude(),atack_point.latitude());
        }
        for (int i=0;i<current_airbases.size();i++)
            current_airbases[i].airbase_point_range = c.get_range(current_airbases[i],atack_point.longitude(),atack_point.latitude());
        for (int i=0;i<current_planes.size();i++)
        {
            current_planes[i].airbase_hit_index.clear();
            for (int j=0;j<current_airbases.size();j++)
            {
                qreal range_plane_airbase = c.get_range(current_planes[i],current_airbases[j]);
                if (range_plane_airbase<current_planes[i].plane_point_range)
                {

                    qreal range_hit = c.get_hit_on_radius(current_planes[i].plane_point_range,current_airbases[j].airbase_point_range,range_plane_airbase,current_airbases[j].Svrvb_vec[i]);
                    if (range_hit>0) //если пролетает через зону и долетел до объекта записываем эффективности поражения
                    {
                        qDebug()<<current_airbases[j].name<<"пересекает траекторию"<<current_planes[i].name<<"параметр = "<<range_hit<<"индекс самолета"<<i+1;
                        current_planes[i].airbase_hit_index.append(j);
                        current_planes[i].hit = true;
                    }
                }
            }
        }
        send_data_in_layer(current_planes, current_airbases,line_atack);
        show_result_window();
    }
    else
        QMessageBox::critical(this,"Ошибка","Заданы не все исходные параметры");
}

void MainWindow::open_DB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = qApp->applicationDirPath();
    qDebug()<<path;
    path.push_back("/database.db");
    db.setDatabaseName(path);
    if (db.open())  qDebug("DB open");
    else qDebug("DB not open");
}

void MainWindow::create_table_position(QTableWidget *&tableWidget, QVector<plane> current_planes, QVector <airbase> current_airbases)
{
    create_table_interface(tableWidget);
    int index = 0;
    foreach (auto p, current_planes)
    {
        tableWidget->setRowCount(tableWidget->rowCount()+1);
        tableWidget->setItem(tableWidget->rowCount()-1,0,new QTableWidgetItem(p.name));
        create_spinBox_course(index,tableWidget,"plane");
        create_button_position(index,tableWidget,"plane");
        index++;
    }
    index = 0;
    foreach (auto a, current_airbases)
    {
        tableWidget->setRowCount(tableWidget->rowCount()+1);
        tableWidget->setItem(tableWidget->rowCount()-1,0,new QTableWidgetItem(a.name));
        create_spinBox_course(index,tableWidget,"airbase");
        create_button_position(index,tableWidget,"airbase");
        index++;
    }

    corretc_table_position_visual(tableWidget,ui->frame_table);
}

void MainWindow::create_spinBox_course(int index, QTableWidget *&tableWidget, QString type)
{
    QDoubleSpinBox* spinBox_course = new QDoubleSpinBox();
    spinBox_course->setMinimum(0);
    spinBox_course->setMaximum(360);
    spinBox_course->setSingleStep(10);
    tableWidget->setCellWidget(tableWidget->rowCount()-1,3,spinBox_course);
    tableWidget->cellWidget(tableWidget->rowCount()-1,3)->setObjectName(QString("%1_%2").arg(type).arg(index));
    connect(spinBox_course,SIGNAL(valueChanged(double)),this,SLOT(on_doubleSpinBox_valueChanged(double)));
}


void MainWindow::create_button_position(int index, QTableWidget *&tableWidget, QString type)
{
    AnimatedLabel* coor_button = new AnimatedLabel(this,":/new/prefix1/icons/coor.gif","Нажмите для ввода координат на карте",47,47);
    tableWidget->setCellWidget(tableWidget->rowCount()-1,4,coor_button);
    tableWidget->cellWidget(tableWidget->rowCount()-1,4)->setObjectName(QString("%1_%2").arg(type).arg(index));
    connect(coor_button,SIGNAL(clicked()),this,SLOT(on_pushButton_coor()));
}

QVector<GeoDataCoordinates> MainWindow::convert_pair_to_coor(QVector<std::pair<qreal, qreal> > vec_of_pair)
{
    QVector<GeoDataCoordinates> vec_for_return;
    foreach (auto p, vec_of_pair)
    {
        GeoDataCoordinates coor(p.first,p.second);
        vec_for_return.append(coor);
    }
    return vec_for_return;
}

bool MainWindow::all_planes_have_coor(QVector<plane> current_planes)
{
    foreach (auto p, current_planes)
    {
        if ((p.lon==0)||(p.lat==0)) return false;
    }
    return true;
}

void MainWindow::draw_legend(QCustomPlot *&widget, QVector<plane> current_planes)
{
    widget->clearGraphs();
    widget->show();
    int height = 30;
    widget->xAxis->setVisible(false);
    widget->yAxis->setVisible(false);
    int plane_index = 1;
    foreach (auto a, current_planes)
    {
        widget->addGraph();
        widget->graph()->setPen(QPen(a.color,50));
        widget->graph()->setName(QString("%1").arg(plane_index));
        widget->replot();
        plane_index++;
        height+=21;
    }
    widget->legend->setVisible(true);
    widget->replot();
    widget->setMaximumHeight(height+30);
}

void MainWindow::on_pushButton_coor()
{
    if (sender()->objectName().split("_")[0]=="plane")
    {
        current_plane_index = sender()->objectName().split("_")[1].toInt();
        connect(map,SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),this,SLOT(get_coordibates_plane(qreal,qreal,GeoDataCoordinates::Unit)));
    }
    else
    {
        current_airbase_index = sender()->objectName().split("_")[1].toInt();
        connect(map,SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),this,SLOT(get_coordibates_airbase(qreal,qreal,GeoDataCoordinates::Unit)));
    }
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    if (sender()->objectName().split("_")[0]=="plane")
        current_planes[sender()->objectName().split("_")[1].toInt()].course=arg1;
    else
        current_airbases[sender()->objectName().split("_")[1].toInt()].course=arg1;
    send_data_in_layer(current_planes, current_airbases,line_atack);
}

void MainWindow::get_coordibates_plane(qreal lon, qreal lat, GeoDataCoordinates::Unit)
{
    ui->tableWidget_position->setItem(current_plane_index,1,new QTableWidgetItem(QString::number(lon*57.2958,'f',2)));
    ui->tableWidget_position->setItem(current_plane_index,2,new QTableWidgetItem(QString::number(lat*57.2958,'f',2)));
    current_planes[current_plane_index].lon = lon;
    current_planes[current_plane_index].lat = lat;

    send_data_in_layer(current_planes,current_airbases,line_atack);

    disconnect(map,SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),this,SLOT(get_coordibates_plane(qreal,qreal,GeoDataCoordinates::Unit)));

}

void MainWindow::get_coordibates_airbase(qreal lon, qreal lat, GeoDataCoordinates::Unit)
{
    ui->tableWidget_position->setItem(current_airbase_index+current_planes.size(),1,new QTableWidgetItem(QString::number(lon*57.2958,'f',2)));
    ui->tableWidget_position->setItem(current_airbase_index+current_planes.size(),2,new QTableWidgetItem(QString::number(lat*57.2958,'f',2)));
    current_airbases[current_airbase_index].lon = lon;
    current_airbases[current_airbase_index].lat = lat;

    send_data_in_layer(current_planes,current_airbases,line_atack);

    disconnect(map,SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),this,SLOT(get_coordibates_airbase(qreal,qreal,GeoDataCoordinates::Unit)));
}

void MainWindow::get_coordibates_line_atack(qreal lon, qreal lat, GeoDataCoordinates::Unit)
{
    GeoDataCoordinates coor(lon,lat,GeoDataCoordinates::Radian);
    line_atack.append(coor);
    if (line_atack.size()==2)
    {
        atack_point = line_atack.last();
        //Обновляем курс исходя из полосы атаки
        current_planes = c.update_all_plane_course(current_planes,line_atack.last().longitude(),line_atack.last().latitude());
        get_line_atack();
        w_calc();
        disconnect(map,SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),this,SLOT(get_coordibates_line_atack(qreal,qreal,GeoDataCoordinates::Unit)));
    }
}

void MainWindow::zoom_map(int value)
{
    layer->zoom_value=value;
}

void MainWindow::get_coor_in_string(QString string)
{
    QString fake_lon = string.split(", ")[0];
    QString fake_lat = string.split(", ")[1];

    fake_lon.replace("° ",".");
    fake_lat.replace("° ",".");
    fake_lon.replace("' ","");
    fake_lat.replace("' ","");
    fake_lon.replace(",","");
    fake_lat.replace(",","");
    fake_lon.replace("\"","");
    fake_lat.replace("\"","");

    qreal lon,lat;
    if (fake_lon.indexOf("E")!=-1)
        lon = fake_lon.replace("E","").toDouble();
    else
        lon = -fake_lon.replace("W","").toDouble();
    if (fake_lat.indexOf("N")!=-1)
        lat = fake_lat.replace("N","").toDouble();
    else
        lat = -fake_lat.replace("S","").toDouble();
}

void MainWindow::mouse_move_on_map(QString string)
{
    if (string!="not available")
    {
        ui->statusbar->showMessage(QString("%1").arg(string));
        get_coor_in_string(string);
    }
    else ui->statusbar->clearMessage();
}

void MainWindow::create_table_interface(QTableWidget *&tableWidget)
{
    tableWidget->setRowCount(0);
    QStringList horiz_header;
    horiz_header<<"Наименование"<<"Широта"<<"Долгота"<<"Курс"<<"";
    tableWidget->setColumnCount(horiz_header.size());
    tableWidget->setHorizontalHeaderLabels(horiz_header);
}

void MainWindow::corretc_table_position_visual(QTableWidget *&tableWidget, QFrame *&frame)
{
    tableWidget->horizontalHeader()->resizeContentsPrecision();
    tableWidget->verticalHeader()->resizeContentsPrecision();
    tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
    tableWidget->setColumnWidth(4,47);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    int table_width = 0;
    for (int i=0;i<tableWidget->columnCount();i++)
    {
        table_width+=tableWidget->columnWidth(i);
    }
    frame->setMaximumWidth(table_width+47+21);
}

void MainWindow::on_pushButton_equipment()
{
    equipwin = new EquipWindow(this);
    equipwin->setWindowModality(Qt::WindowModal);
    connect(equipwin,SIGNAL(send_data(QVector<plane>, QVector <airbase>)),this,SLOT(recive_data(QVector<plane>, QVector <airbase>)));
    equipwin->show();
}

void MainWindow::on_pushButton_settings()
{
    settingswin = new SettingsWindow(this, map->mapThemeId(), map->projection());
    settingswin->setWindowModality(Qt::WindowModal);
    connect(settingswin,SIGNAL(send_map_settings(QString, int)),this,SLOT(set_map_settings(QString, int)));
    settingswin->show();
}

void MainWindow::on_pushButton_result()
{
    w_calc();
}

void MainWindow::set_map_settings(QString map_theme, int projection)
{
    delete map;
    load_map(ui->map_lay,map_theme,projection);
    send_data_in_layer(current_planes, current_airbases,line_atack);
}

void MainWindow::recive_data(QVector<plane> cur_planes, QVector<airbase> cur_airbases)
{
    if ((cur_planes.size()==0)||(cur_airbases.size()==0))
    {
       QMessageBox::warning(this,"Внимание","Вы не выбрали никакого оснащения");
    }
    else
    {
        ui->frame_table->show();
        line_atack.clear();
        layer->l_atack.clear();
        layer->cur_planes.clear();
        layer->cur_airbases.clear();
        current_airbases = cur_airbases;
        current_planes = cur_planes;

        create_table_position(ui->tableWidget_position,current_planes,current_airbases);

        draw_legend(ui->widget_legend,current_planes);

    }

}

void MainWindow::on_pushButton_line()
{
    if (all_planes_have_coor(current_planes))
    {
        //current_planes = c.count_all_ranges_plane_airbase(current_planes,current_airbases);
        line_atack.clear();
        connect(map,SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),this,SLOT(get_coordibates_line_atack(qreal,qreal,GeoDataCoordinates::Unit)));
    }
    else
        QMessageBox::critical(this,"Ошибка","Сначала задайте координаты всех самолетов");
}
