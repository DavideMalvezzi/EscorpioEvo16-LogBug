#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QSettings>

#include "logmodel.h"
#include "logfiltermodel.h"
#include "serialreader.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void loadSerialPortParams();
    ~MainWindow();

private slots:
    void on_actionReload_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionPause_triggered();
    void on_actionSettings_triggered();
    void on_cancelButton_clicked();
    void on_okButton_clicked();

    void addToLog(LogRow* row);
    void updateFilterParams();

    void on_actionClear_triggered();

private:
    Ui::MainWindow *ui;
    QComboBox* portCombo;
    LogModel* logModel;
    LogFilterModel* logFilterModel;
    SerialReader* serialReader;
};

#endif // MAINWINDOW_H
