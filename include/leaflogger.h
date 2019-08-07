#ifndef LEAFLOGGER_H
#define LEAFLOGGER_H

#include "leaflogger_global.h"

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>
#include <QTextCodec>
#include <QSysInfo>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QQueue>
#include "asyncfilewriter.h"
#include <QCoreApplication>

class LEAFLOGGERSHARED_EXPORT LeafLogger
{
public:
    static void LogMessage(const QString log);
    static void LogMessage(QMetaObject* metaObject,const QString log);
    static void setFileName(QString name);
    static QString setFileNameWithTime(const QString timeFormat = "yyyyMMddhhmmsszzz");
    LeafLogger& operator<<(const QString log);
    static void LogSysInfo();
    static void LogInit();
    static void messageHandler(QtMsgType msgType, const QMessageLogContext& messageLogContext, const QString& message);
    static QString getFileName();

private:
    static QMutex consoleMutex;
    static void LogMessagePrivate(const QString log);
    static QString getLogWithTime(const QString& log);
    static void commitLog(const QString& log);
    static int printToConsole(const QString& log);
    static AsyncFileWriter* logFileWriter;
    class Garbo{
    public:
        Garbo();
        ~Garbo();

    };
    static Garbo garbo;
    friend Garbo;

};


#endif // LEAFLOGGER_H
