#include "equipwindow.h"
#include "ui_equipwindow.h"

EquipWindow::EquipWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EquipWindow)
{
    ui->setupUi(this);

    //Db
    open_DB();

    //buttons
    create_button_data();
    create_button_add();

    //table
    create_table_current(ui->tableWidget_current);
    create_table_airbase(ui->tableWidget_airbase);

    //listWidget data
    add_item_in_data_listWidget(load_DB("planes"),ui->listWidget_data);
}

EquipWindow::~EquipWindow()
{
    delete ui;
}

QVector<airbase> EquipWindow::create_current_airbases(QTableWidget *&tableWidget, QVector<plane> cur_planes)
{
    QVector <airbase> cur_air;
    if (tableWidget->rowCount()==0) return cur_air;
    else
    {
        for (int i=0;i<tableWidget->rowCount();i++)
        {
            airbase a;
            a.name = tableWidget->item(i,0)->text();
            a.lon = 0;
            a.lat = 0;
            a.course = 0;
            foreach (auto S, cur_planes)
            {
                a.Svrvb_vec.append(S.Svrvb);
            }
            cur_air.append(a);
        }
        return cur_air;
    }
}

void EquipWindow::create_button_data()
{
    AnimatedLabel* dataButton = new AnimatedLabel(this,":/new/prefix1/icons/database.gif","Нажмите для редактирования базы данных типовых самолетов",64,64);
    ui->toolBar->addWidget(dataButton);
    connect(dataButton,SIGNAL(clicked()),SLOT(on_pushButton_data()));
}

void EquipWindow::create_button_add()
{
    AnimatedLabel* addButton = new AnimatedLabel(this,":/new/prefix1/icons/add.gif","Нажмите для добавления самолета в текущее оснащение",128,128);
    ui->lay_but_add->addWidget(addButton);
    connect(addButton,SIGNAL(clicked()),SLOT(on_pushButton_add()));
}

void EquipWindow::add_item_in_data_listWidget(QVector<plane> data, QListWidget *&listWidget)
{
    listWidget->clear();
    QStringList items;
    foreach (auto item, data)
        items.append(item.name);
    listWidget->addItems(items);
}

void EquipWindow::insert_data_in_parms_on_click(QString name, QVector<plane> data)
{
    int index_of_cur_plane = index_of_plane(name,data);
    ui->doubleSpinBox_V->setValue(data.value(index_of_cur_plane).Vc);
    ui->doubleSpinBox_S->setValue(data.value(index_of_cur_plane).Sish);
    ui->doubleSpinBox_D->setValue(data.value(index_of_cur_plane).Dk);
}

void EquipWindow::create_table_current(QTableWidget *&tableWidget)
{
    QStringList horiz_header;
    horiz_header<<"Наименование"
               <<"Высота\nатаки"
              <<"Атака в"
             <<"Vц, км/ч"
            <<"Sисх, км"
           <<"tΣ, мин"
          <<"SΣ, км"
         <<"Dk, км"
        <<"Sврвб, км";
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(horiz_header.size());
    tableWidget->setHorizontalHeaderLabels(horiz_header);
    correct_visual(tableWidget);
}

void EquipWindow::correct_visual(QTableWidget *&tableWidget)
{
    tableWidget->horizontalHeader()->resizeContentsPrecision();
    tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableWidget->verticalHeader()->resizeContentsPrecision();
    tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();
}

void EquipWindow::update_table(QTableWidget *&tableWidget, QVector<plane> data)
{
    tableWidget->setRowCount(data.size());
    int i=0;
    foreach (auto p, data)
    {
        tableWidget->setItem(i,0,new QTableWidgetItem(p.name));
        tableWidget->setItem(i,1,new QTableWidgetItem(ui->comboBox_alt->itemText(p.alt_type)));
        tableWidget->setItem(i,2,new QTableWidgetItem(ui->comboBox_sfer->itemText(p.sfer_type)));
        tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(p.Vc)));
        tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(p.Sish)));
        tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(p.tsum)));
        tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(p.Ssum)));
        tableWidget->setItem(i,7,new QTableWidgetItem(QString::number(p.Dk)));
        tableWidget->setItem(i,8,new QTableWidgetItem(QString::number(p.Svrvb)));
        i++;
    }
}

void EquipWindow::create_table_airbase(QTableWidget *&tableWidget)
{
    QStringList horiz_header;
    horiz_header<<"Наименоваине\nаэродрома";
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(horiz_header.size());
    tableWidget->setHorizontalHeaderLabels(horiz_header);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QVector<plane> EquipWindow::load_DB(QString table_name)
{
    QVector <plane> data;

    query = new QSqlQuery(db);
    query->exec(QString("SELECT * FROM %1;").arg(table_name));
    while (query->next())
    {
        plane p;
        p.name  = query->value(0).toString();
        p.tpass = query->value(1).toDouble();
        p.tn    = query->value(2).toDouble();
        p.tr    = query->value(3).toDouble();
        p.t180  = query->value(4).toDouble();
        p.tvm   = query->value(5).toDouble();
        p.Sn    = query->value(6).toDouble();
        p.Sr    = query->value(7).toDouble();
        p.Lsm   = query->value(8).toDouble();
        p.Svm   = query->value(9).toDouble();
        p.Vc    = query->value(10).toDouble();
        p.Sish  = query->value(11).toDouble();
        p.Dk    = query->value(12).toDouble();
        p.Vgp   = query->value(13).toDouble();

        data.push_back(p);
    }
    return data;
}

void EquipWindow::open_DB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = qApp->applicationDirPath();
    path.push_back("/database.db");
    db.setDatabaseName(path);
    if (db.open())  qDebug("DB open");
    else qDebug("DB not open");
}

int EquipWindow::index_of_plane(QString name, QVector<plane> data)
{
    for (int i=0;i<data.size();i++)
        if (data.value(i).name == name)
            return i;
}

void EquipWindow::closeEvent(QCloseEvent *event)
{
    current_airbases = create_current_airbases(ui->tableWidget_airbase,current_planes);
    emit send_data(current_planes, current_airbases);
}

void EquipWindow::update_slot()
{
    qDebug()<<"Слот обновления";
    add_item_in_data_listWidget(load_DB("planes"),ui->listWidget_data);
}

void EquipWindow::on_pushButton_data()
{
    datawin = new DataWindow(this);
    datawin->setWindowModality(Qt::WindowModal);
    connect(datawin,SIGNAL(update_window()),this,SLOT(update_slot()));
    datawin->showMaximized();
}

void EquipWindow::on_pushButton_add()
{
    if (currentName.isEmpty())
        QMessageBox::critical(this,"Ошибка","Выберите один из типовых вариантов оснащения!");
    else
    {
        if ((ui->doubleSpinBox_D->value()==0)||(ui->doubleSpinBox_S->value()==0)||(ui->doubleSpinBox_V->value()==0))
            QMessageBox::critical(this,"Ошибка","Нулевые параметры не возможны!");
        else
        {
            current_planes.append(c.create_current_plane(load_DB("planes").value(index_of_plane(currentName,load_DB("planes"))),
                                                             currentName,
                                                             ui->comboBox_alt->currentIndex(),
                                                             ui->comboBox_sfer->currentIndex(),
                                                             ui->doubleSpinBox_V->value(),
                                                             ui->doubleSpinBox_S->value(),
                                                             ui->doubleSpinBox_D->value()));
            update_table(ui->tableWidget_current,current_planes);
        }
    }
}

void EquipWindow::on_tableWidget_current_cellDoubleClicked(int row, int column)
{
    ui->tableWidget_current->removeRow(row);
    current_planes.remove(row);
    update_table(ui->tableWidget_current,current_planes);
}

void EquipWindow::on_tableWidget_current_cellClicked(int row, int column)
{
    ui->tableWidget_current->selectRow(row);
    update_table(ui->tableWidget_current,current_planes);
}

void EquipWindow::on_listWidget_data_currentTextChanged(const QString &currentText)
{
    currentName = currentText;
    insert_data_in_parms_on_click(currentName,load_DB("planes"));
}

void EquipWindow::on_spinBox_airbase_valueChanged(int arg1)
{
    ui->tableWidget_airbase->setRowCount(arg1);
    for (int i=0;i<ui->tableWidget_airbase->rowCount();i++)
    {
        qDebug()<<i;
        ui->tableWidget_airbase->setItem(i,0,new QTableWidgetItem(QString("Аэродром №%1").arg(i+1)));
    }

}
