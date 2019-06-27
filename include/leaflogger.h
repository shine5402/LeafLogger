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
class LEAFLOGGERSHARED_EXPORT LeafLogger
{
public:
    static void LogMessage(const QString log);
    static void LogMessage(QMetaObject* metaObject,const QString log);
    static void setFilePath(QString Path);
    static QString setFilePathWithTime(const QString timeFormat = "yyyyMMddhhmmsszzz");
    LeafLogger& operator<<(const QString log);
    static LeafLogger getLogger();
    static void LogSysInfo();
    static void LogInit();
    static void messageHandler(QtMsgType msgType, const QMessageLogContext& messageLogContext, const QString& message);
    static QString getFileName();
signals:

public slots:

private:
    static QFile file;
    static bool isFileSetPath;
    static QMutex mutex;
};


#endif // LEAFLOGGER_H
