#include "datawindow.h"
#include "ui_datawindow.h"

DataWindow::DataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataWindow)
{
    ui->setupUi(this);

    //buttons
    create_button_add();
    create_button_remove();
    //tableView
    open_model("planes", ui->tableView);

}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::create_button_add()
{
    AnimatedLabel* addButton = new AnimatedLabel(this,":/new/prefix1/icons/edit.gif","Нажмите для создания типового варианта самолета",64,64);
    ui->lay_but_add->addWidget(addButton);
    connect(addButton,SIGNAL(clicked()),SLOT(on_pushButton_add()));
}

void DataWindow::create_button_remove()
{
    AnimatedLabel* removeButton = new AnimatedLabel(this,":/new/prefix1/icons/remove.gif","Нажмите для создания удаления типового варианта самолета",64,64);
    ui->lay_but_remove->addWidget(removeButton);
    connect(removeButton,SIGNAL(clicked()),SLOT(on_pushButton_remove()));
}

void DataWindow::open_DB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = qApp->applicationDirPath();
    qDebug()<<path;
    path.push_back("/database.db");
    db.setDatabaseName(path);
    if (db.open())  qDebug("DB open");
    else qDebug("DB not open");
}

void DataWindow::open_model(QString table_name, QTableView *&tableView)
{
    open_DB();
    model = new QSqlTableModel(this,db);
    model->setTable(table_name);
    model->select();
    tableView->setModel(model);
    QStringList horiz_header;
    horiz_header<<"Наименование"
               <<"tпасс, мин"
              <<"tн, мин"
             <<"tр, мин"
            <<"t180, мин"
           <<"tвм, мин"
          <<"Sн, км"
         <<"Sр, км"
        <<"Lсм"
       <<"Sвм, км"
      <<"Vц, км/ч"
     <<"Sисх, км"
    <<"Dk, км"
   <<"Vгп, км/ч";
    int i=0;
    foreach (QString str, horiz_header)
    {
        model->setHeaderData(i,Qt::Horizontal,str);
        i++;
    }
    correct_visual(tableView);
}

void DataWindow::correct_visual(QTableView *&tableView)
{
    tableView->horizontalHeader()->resizeContentsPrecision();
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->verticalHeader()->resizeContentsPrecision();
    tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();
}

void DataWindow::closeEvent(QCloseEvent *event)
{
    emit update_window();
}

void DataWindow::on_pushButton_add()
{
    model->insertRow(model->rowCount());
}

void DataWindow::on_pushButton_remove()
{
    if (row_to_remove==-1)
    {
        QMessageBox::critical(this,"Ошибка","Выберите строку для удаления");
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning,"Внимание",QString("Вы действительно хотите удалить: %1").arg(model->data(model->index(row_to_remove,0),Qt::DisplayRole).toString()),
                                                    QMessageBox::Yes| QMessageBox::No);
        if(msgBox->exec() == QMessageBox::Yes)
        {
            model->removeRow(row_to_remove);
        }
        delete msgBox;
    }
}

void DataWindow::on_tableView_clicked(const QModelIndex &index)
{
    row_to_remove=index.row();
}
