#ifndef LOGROW_H
#define LOGROW_H

#include <QString>
#include <QTime>

enum LogLevel{
    Info = 0x11,
    Warning = 0x12,
    Error = 0x13
};

class LogRow {

public:
    LogRow();

    QString getTime(){ return time; }
    QString getTag(){ return tag; }
    QString getMessage(){ return msg; }
    LogLevel getLogLevel(){ return level; }
    QString getLogLevelString();

    void setTimeNow(){this->time = QTime::currentTime().toString("hh:mm:ss:zzz");}
    void setTag(QString tag){ this->tag = tag; }
    void setMessage(QString msg){ this->msg = msg; }
    void setLogLevel(LogLevel level){ this->level = level; }

private:
    QString time, tag, msg;
    LogLevel level;

};

#endif // LOGROW_H
