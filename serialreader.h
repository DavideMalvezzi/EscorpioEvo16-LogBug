#ifndef SERIALREADER_H
#define SERIALREADER_H


#include <QThread>
#include <QSerialPort>
#include <QDebug>

#include "logrow.h"

class SerialReader : public QThread {
    Q_OBJECT

public:

    SerialReader(QObject* parent = 0);
    QSerialPort* getPort(){return port;}

public slots:
    void onDataReceived();

    bool play();
    void pause();
    void stop();

signals:
    void packetReceived(LogRow*);

private:
    bool isPaused;
    QSerialPort* port;
    QByteArray rxBuffer;
};

#endif // SERIALREADER_H
