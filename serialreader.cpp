#include "serialreader.h"

#include <QDebug>
#include <QMessageBox>

SerialReader::SerialReader(QObject* parent) : QThread(parent){
    this->port = new QSerialPort(this);
    this->isPaused = false;

    connect(port, SIGNAL(readyRead()), this, SLOT(onDataReceived()), Qt::QueuedConnection);
}

bool SerialReader::play(){

    if(isPaused && port->isOpen()){
        isPaused = false;
    }
    else{
        port->setDataTerminalReady(false); //needed for native usb arduino due
        if(port->open(QSerialPort::ReadWrite)){
            isPaused = false;
            port->setDataTerminalReady(true); //needed for native usb arduino due
        }
        else{
            QMessageBox::critical(NULL, "Error", QString("Error on opening the serial port: %1").arg(port->errorString()));
            return false;
        }
    }

    return true;
}

void SerialReader::pause(){
    isPaused = true;
}

void SerialReader::stop(){
    port->close();
}

void SerialReader::onDataReceived(){

    if(!isPaused){
        rxBuffer.append(port->readAll());

        for(int i = 0; i < rxBuffer.length(); i++){
            unsigned char ll = rxBuffer.at(i);

            if(ll == Info || ll == Warning || ll == Error){

                //qDebug() << "found ll";
                //qDebug() << rxBuffer.toHex();

                int tagEnd = rxBuffer.indexOf(' ', i);
                int endLine = rxBuffer.indexOf("\r\n", i);

                //qDebug() << tagEnd << "  " << endLine;

                if(tagEnd != -1 && endLine != -1){
                    QString tag, msg;
                    LogRow* row = new LogRow;
                    row->setTimeNow();
                    row->setLogLevel(static_cast<LogLevel>(ll));

                    for(int j = i + 1; j < tagEnd; j++){
                        tag += rxBuffer.at(j);
                    }
                    row->setTag(tag);

                    for(int j = tagEnd + 1; j < endLine; j++){
                        msg += rxBuffer.at(j);
                    }
                    row->setMessage(msg);

                    emit packetReceived(row);

                    rxBuffer.remove(0, endLine + 2 - i + 1);

                    i--;

                    qDebug() << "Read " << row->getLogLevelString() << " " << row->getTag() << " " << row->getMessage();
                }

            }
        }


    }else{
        rxBuffer.clear();
        port->readAll();
    }

}

