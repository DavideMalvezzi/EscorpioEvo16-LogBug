#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    loadSerialPortParams();
    on_actionSettings_triggered();
    ui->stackedWidget->setCurrentIndex(0);

    portCombo = new QComboBox;
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions().first(), portCombo);
    on_actionReload_triggered();

    connect(ui->infoCheck, SIGNAL(toggled(bool)), this, SLOT(updateFilterParams()));
    connect(ui->warningCheck, SIGNAL(toggled(bool)), this, SLOT(updateFilterParams()));
    connect(ui->errorCheck, SIGNAL(toggled(bool)), this, SLOT(updateFilterParams()));
    connect(ui->tagFilterText, SIGNAL(textChanged(QString)), this, SLOT(updateFilterParams()));

    logModel = new LogModel(this);
    logFilterModel = new LogFilterModel(this);
    logFilterModel->setSourceModel(logModel);
    logFilterModel->setDynamicSortFilter(true);
    ui->logTable->setModel(logFilterModel);

    int c;
    for (c = 0; c < ui->logTable->horizontalHeader()->count() - 1; c++){
        ui->logTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    }
    ui->logTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);

    updateFilterParams();

    serialReader = new SerialReader(this);
    connect(serialReader, SIGNAL(packetReceived(LogRow*)), this, SLOT(addToLog(LogRow*)));
    serialReader->start();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionReload_triggered(){
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    portCombo->clear();
    for(int i=0; i<ports.size(); i++){
        portCombo->addItem(ports.at(i).portName());
    }
}

void MainWindow::on_actionOpen_triggered(){
    if(portCombo->currentIndex() != -1){
        QSerialPort* port = serialReader->getPort();
        port->setPortName(portCombo->currentText());
        port->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudRateBox->currentData().toInt()));
        port->setDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsBox->currentData().toInt()));
        port->setParity(static_cast<QSerialPort::Parity>(ui->parityBox->currentData().toInt()));
        port->setStopBits(static_cast<QSerialPort::StopBits>(ui->stopBitsBox->currentData().toInt()));
        port->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->flowControlBox->currentData().toInt()));
        if(serialReader->play()){
            ui->actionOpen->setVisible(false);
            ui->actionPause->setVisible(true);
            ui->actionClose->setEnabled(true);
        }
    }else{
        QMessageBox::critical(this, "Error", "No serial port selected");
    }
}

void MainWindow::on_actionClose_triggered(){
    serialReader->stop();
    ui->actionOpen->setVisible(true);
    ui->actionPause->setVisible(false);
    ui->actionClose->setEnabled(false);
    ui->actionRecord->setEnabled(false);
}

void MainWindow::on_actionPause_triggered(){
    serialReader->pause();
    ui->actionOpen->setVisible(true);
    ui->actionPause->setVisible(false);
}

void MainWindow::on_actionSettings_triggered(){
    QSettings settings("LogBug.ini", QSettings::IniFormat);

    settings.beginGroup("Serialport");
    ui->baudRateBox->setCurrentIndex(settings.value("baud", 0).toInt());
    ui->stopBitsBox->setCurrentIndex(settings.value("stopbits", 0).toInt());
    ui->dataBitsBox->setCurrentIndex(settings.value("databits", 3).toInt());
    ui->parityBox->setCurrentIndex(settings.value("parity", 0).toInt());
    ui->flowControlBox->setCurrentIndex(settings.value("flowcontrol", 0).toInt());
    settings.endGroup();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_cancelButton_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::loadSerialPortParams(){
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

    ui->parityBox->addItem("None", QSerialPort::NoParity);
    ui->parityBox->addItem("Even", QSerialPort::EvenParity);
    ui->parityBox->addItem("Odd", QSerialPort::OddParity);
    ui->parityBox->addItem("Mark", QSerialPort::MarkParity);
    ui->parityBox->addItem("Space", QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem("None", QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem("RTS/CTS", QSerialPort::HardwareControl);
    ui->flowControlBox->addItem("XON/XOFF", QSerialPort::SoftwareControl);
}

void MainWindow::on_okButton_clicked(){
    QSettings settings("LogBug.ini", QSettings::IniFormat);

    settings.beginGroup("Serialport");
        settings.setValue("baud", ui->baudRateBox->currentIndex());
        settings.setValue("databits", ui->dataBitsBox->currentIndex());
        settings.setValue("parity", ui->parityBox->currentIndex());
        settings.setValue("stopbits", ui->stopBitsBox->currentIndex());
        settings.setValue("flowcontrol", ui->flowControlBox->currentIndex());
    settings.endGroup();

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::addToLog(LogRow *row){
    logModel->addRow(row);
    //ui->logTable->resizeColumnsToContents();
    if(ui->actionScroll->isChecked()){
       ui->logTable->selectRow(ui->logTable->model()->rowCount() - 1);
    }
}

void MainWindow::updateFilterParams(){
    logFilterModel->updateFilter(
                ui->infoCheck->isChecked(),
                ui->warningCheck->isChecked(),
                ui->errorCheck->isChecked(),
                ui->tagFilterText->text()
                );
}

void MainWindow::on_actionClear_triggered(){
    logModel->clear();
}
