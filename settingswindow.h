#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <includes.h>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent, QString map_theme_id, int projection);
    ~SettingsWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SettingsWindow *ui;

signals:
    void send_map_settings(QString map_theme, int projection);
};

#endif // SETTINGSWINDOW_H
