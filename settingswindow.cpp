#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, QString map_theme_id, int projection) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    qDebug()<<map_theme_id<<projection;

    QDir dir("./data/maps/earth");
    QFileInfoList dirContent = dir.entryInfoList(QStringList()
            << "*",
            QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList list_of_theme;
    foreach (auto d, dirContent)
        list_of_theme.append(QString("earth/%1/%1.dgml").arg(d.fileName()));

    ui->listWidget_theme->addItems(list_of_theme);
    ui->listWidget_theme->setCurrentRow(list_of_theme.indexOf(map_theme_id));

    QStringList list_of_projection;
    list_of_projection<<"Сферическая"
                         <<"Меркатор"
                           <<"Плоская";
    ui->listWidget_projection->addItems(list_of_projection);
    ui->listWidget_projection->setCurrentRow(projection);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_pushButton_clicked()
{
    emit send_map_settings(ui->listWidget_theme->currentItem()->text(),ui->listWidget_projection->currentRow());

    this->close();
}
