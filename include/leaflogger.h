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
#include "logfilewriter.h"
#include <QCoreApplication>

class LeafLoggerAboutToQuitHelper : public QObject{
    Q_OBJECT
public:
    LeafLoggerAboutToQuitHelper(QObject* parent = nullptr);
    ~LeafLoggerAboutToQuitHelper();

public slots:
    void handleAboutToQuit();
};

class LEAFLOGGERSHARED_EXPORT LeafLogger
{
public:
    static void LogMessage(const QString log);
    static void LogMessage(QMetaObject* metaObject,const QString log);
    static void setFilePath(QString Path);
    static QString setFilePathWithTime(const QString timeFormat = "yyyyMMddhhmmsszzz");
    LeafLogger& operator<<(const QString log);
    static void LogSysInfo();
    static void LogInit(QCoreApplication* coreApplication = nullptr);
    static void messageHandler(QtMsgType msgType, const QMessageLogContext& messageLogContext, const QString& message);
    static QString getFileName();

private:
    static QFile file;
    static bool isFileSetPath;
    static QMutex consoleMutex;
    static void LogMessagePrivate(const QString log);
    static QString getLogWithTime(const QString& log);
    static void commitLog(const QString& log);
    static int printToConsole(const QString& log);
    static QQueue<QString> logBuffer;
    static QMutex logBufferMutex;
    static void addToBuffer(const QString& log);
    static void writeToFile(QString log);
    static QList<LogFileWriterController *> controllerList;
    class Garbo{
    public:
        Garbo();
        ~Garbo();

    };
    static Garbo garbo;
    friend Garbo;
    friend LeafLoggerAboutToQuitHelper;
    static LeafLoggerAboutToQuitHelper* aboutToQuitHelper;
};


#endif // LEAFLOGGER_H
