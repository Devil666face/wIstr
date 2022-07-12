#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QSS
    QFile styleSheetFile(":/new/prefix1/HackBook.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);
    //Localization
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
