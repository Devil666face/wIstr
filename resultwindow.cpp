#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent, QVector <plane> current_planes, QVector<airbase> current_airbase) :
    QMainWindow(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    create_destroy_widget(ui->lay_for_table,current_planes,current_airbase);
    update_frame_height(row_heights,ui->frame_plane_table);
    draw_graph(ui->widget_graph,current_planes,current_airbase);

}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::create_destroy_widget(QHBoxLayout *&lay, QVector<plane> current_planes, QVector<airbase> current_airbase)
{
    for (int j=0;j<current_airbase.size();j++)
    {
        lay->addWidget(create_airbase_table(current_planes,current_airbase,j));
    }
    lay->addWidget(create_result_table(current_planes));


}

void ResultWindow::correct_visual(QTableWidget *&tableWidget)
{
    tableWidget->horizontalHeader()->resizeContentsPrecision();
    tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableWidget->verticalHeader()->resizeContentsPrecision();
    tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
}

QTableWidget* ResultWindow::create_result_table(QVector<plane> current_planes)
{
    QTableWidget *tableWidget_params = new QTableWidget();
    QStringList horiz_header;
    tableWidget_params->setRowCount(3);
    tableWidget_params->setColumnCount(2);
    horiz_header<<"Параметр"<<"Значение";
    tableWidget_params->setHorizontalHeaderLabels(horiz_header);
    tableWidget_params->verticalHeader()->hide();
    tableWidget_params->setItem(0,0, new QTableWidgetItem("Всего самолетов"));
    tableWidget_params->setItem(0,1, new QTableWidgetItem(QString("%1").arg(current_planes.size())));
    tableWidget_params->setItem(1,0, new QTableWidgetItem("Уничтожено"));
    tableWidget_params->setItem(1,1, new QTableWidgetItem(QString("%1").arg(get_count_destroy_planes(current_planes))));
    tableWidget_params->setItem(2,0, new QTableWidgetItem("Эффективность"));
    tableWidget_params->setItem(2,1, new QTableWidgetItem(QString::number(get_w(get_count_destroy_planes(current_planes),current_planes.size()),'f',2)));
    correct_visual(tableWidget_params);
    row_heights.append(get_row_height(tableWidget_params));
    return tableWidget_params;

}

QTableWidget *ResultWindow::create_airbase_table(QVector<plane> current_planes, QVector<airbase> current_airbase, int j)
{
    QTableWidget *tableWidget_airbase = new QTableWidget();
    QStringList horiz_header;
    tableWidget_airbase->setRowCount(0);
    tableWidget_airbase->setColumnCount(1);
    horiz_header<<current_airbase[j].name;
    tableWidget_airbase->setHorizontalHeaderLabels(horiz_header);
    int plane_index = 1;
    foreach (auto p, current_planes)
    {
        foreach (auto i, p.airbase_hit_index)
        {
            if (i==j)
            {
                tableWidget_airbase->setRowCount(tableWidget_airbase->rowCount()+1);
                tableWidget_airbase->setItem(tableWidget_airbase->rowCount()-1,0,new QTableWidgetItem(QString("Индекс %1\n%2").arg(plane_index).arg(p.name)));
            }
        }
        plane_index++;
    }
    correct_visual(tableWidget_airbase);
    row_heights.append(get_row_height(tableWidget_airbase));
    return tableWidget_airbase;
}

int ResultWindow::get_count_destroy_planes(QVector<plane> current_planes)
{
    int destroy_planes = 0;
    foreach (auto p,current_planes)
    {
        if (p.airbase_hit_index.size()!=0)
            destroy_planes++;
    }
    return destroy_planes;
}

int ResultWindow::get_row_height(QTableWidget *&tableWidget)
{
    int row_height = 0;
    for (int i=0;i<tableWidget->rowCount();i++)
    {
        row_height+=tableWidget->rowHeight(i);
    }
    return row_height;
}

qreal ResultWindow::get_w(int destroy, int all)
{
    qreal dest = destroy;
    qreal size = all;
    return dest/size;
}

void ResultWindow::update_frame_height(QVector<int> row_heights, QFrame *&frame)
{
    qSort(row_heights);
    qDebug()<<"Высота фрейма"<<row_heights.last();
    frame->setMaximumHeight(row_heights.last()+95);
}

void ResultWindow::draw_graph(QCustomPlot *&widget, QVector<plane> current_planes, QVector<airbase> current_airbase)
{
    QVector <int> hit_counts;
    widget->setInteraction(QCP::iRangeZoom,true);
    widget->setInteraction(QCP::iRangeDrag,true);
    widget->xAxis->setLabel("Номер аэродрома");
    widget->yAxis->setLabel("Количество уничтоженных самолетов");
    widget->xAxis->setRange(0,current_airbase.size()+1);
    widget->yAxis->setRange(0,get_max_el(get_hit_counts(current_planes,current_airbase))+1);
    QVector <int> y = get_hit_counts(current_planes,current_airbase);
    QVector <int> x = get_x_vector(current_airbase);
    for (int i=0;i<x.size();i++)
    {
        int r = Random::get(0,255);
        int g = Random::get(0,255);
        int b = Random::get(0,255);
        qreal x_one = x.at(i);
        qreal y_one = y.at(i);
        widget->addGraph();
        widget->graph()->addData(x_one,y_one);
        widget->graph()->setPen(QPen(QColor(r,g,b,200),100));
        widget->graph()->setLineStyle((QCPGraph::LineStyle)(5));
        widget->graph()->setName(get_airbase_names(current_airbase).at(i));
        widget->replot();
    }
    widget->legend->setVisible(true);
    widget->replot();
}

QVector<int> ResultWindow::get_hit_counts(QVector<plane> current_planes, QVector<airbase> current_airbase)
{
    QVector <int> hit_counts;
    for (int j=0;j<current_airbase.size();j++)
    {
        int hit_count = 0;
        foreach (auto p, current_planes)
        {
            foreach (auto i, p.airbase_hit_index)
            {
                if (i==j)
                {
                    hit_count++;
                }
            }
        }
        qDebug()<<"Сбито самолетов"<<hit_count;
        hit_counts.append(hit_count);
    }
    return hit_counts;
}

int ResultWindow::get_max_el(QVector<int> vector)
{
    qSort(vector);
    return vector.last();
}

QVector<int> ResultWindow::get_x_vector(QVector<airbase> current_airbase)
{
    QVector <int> vec_for_return;
    int i=1;
    foreach (auto iter, current_airbase) {
        vec_for_return.append(i);
        i++;
    }
    return vec_for_return;
}

QStringList ResultWindow::get_airbase_names(QVector<airbase> current_airbase)
{
    QStringList airbase_names;
    foreach (auto a,current_airbase)
        airbase_names.append(a.name);
    return airbase_names;
}



