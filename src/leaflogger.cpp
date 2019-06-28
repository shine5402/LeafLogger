﻿#include "../include/leaflogger.h"
#include <cstdio>

QString LeafLogger::getLogWithTime(const QString& log)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    return QString(u8"[%1] %2\n").arg(current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz")).arg(log);
}

void LeafLogger::commitLog(const QString& log)
{
    printToConsole(log);
    addToBuffer(log);
}

int LeafLogger::printToConsole(const QString& log)
{
    QMutexLocker locker(&consoleMutex);
    return std::fprintf(stderr,log.toUtf8().data());
}

void LeafLogger::addToBuffer(const QString& log)
{
    QMutexLocker locker(&logBufferMutex);
    logBuffer.enqueue(log);
    emit logFileWriterController->bufferReady();
}

void LeafLogger::LogMessagePrivate(const QString log)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString log_text = getLogWithTime(log);
    commitLog(log_text);
}


void LeafLogger::LogMessage(const QString log)
{
    LogMessagePrivate(QString("[Direct] %1").arg(log));
}

void LeafLogger::LogMessage(QMetaObject *metaObject, const QString log)
{
    LogMessagePrivate(QString("[Log from %1] %2").arg(metaObject->className()).arg(log));
}

void LeafLogger::setFilePath(QString Path)
{
    if (file.isOpen())
        file.close();
    file.setFileName(Path);
    file.open(QIODevice::ReadWrite);
    isFileSetPath = true;
}

QString LeafLogger::setFilePathWithTime(const QString timeFormat)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QDir* dir = new QDir;
    if (!dir->exists("./log")){
        dir->mkdir("log");
    }
    delete dir;
    auto fileName=QString("./log/log%1.txt").arg(current_date_time.toString(timeFormat));
    setFilePath(fileName);
    return fileName;
}
void LeafLogger::LogSysInfo()
{
    LeafLogger::LogMessagePrivate("======  System Info  ======");
    LeafLogger::LogMessagePrivate(QString(u8"bootUniqueId: %1").arg(QString(QSysInfo::bootUniqueId())));
    LeafLogger::LogMessagePrivate(QString(u8"buildAbi: %1").arg(QSysInfo::buildAbi()));
    LeafLogger::LogMessagePrivate(QString(u8"currentCpuArchitecture: %1").arg(QSysInfo::currentCpuArchitecture()));
    LeafLogger::LogMessagePrivate(QString(u8"machineUniqueId: %1").arg(QString(QSysInfo::machineUniqueId())));
    LeafLogger::LogMessagePrivate(QString(u8"machineHostName: %1").arg(QSysInfo::machineHostName()));
    LeafLogger::LogMessagePrivate(QString(u8"prettyProductName: %1").arg(QSysInfo::prettyProductName()));
    LeafLogger::LogMessagePrivate("======  System Info End  ======");
}

void LeafLogger::LogInit()
{
    setFilePathWithTime();
    LogSysInfo();
    qInstallMessageHandler(messageHandler);
}

void LeafLogger::messageHandler(QtMsgType msgType, const QMessageLogContext& messageLogContext, const QString& message)
{
    QString msgTypeString;
    switch (msgType) {
    case QtDebugMsg :
        msgTypeString = "[Debug] ";
        break;
    case QtInfoMsg :
        msgTypeString = "[Info] ";
        break;
    case QtWarningMsg :
        msgTypeString = "[Warning] ";
        break;
    case QtCriticalMsg :
        msgTypeString = "[Critical] ";
        break;
    case QtFatalMsg :
        msgTypeString = "[Fatal] ";
    }
    QString logContextString = QString("\n========================= category: %1 | file: %2 line: %3\n========================= function : %4 | version : %5").arg(messageLogContext.category).arg(messageLogContext.file).arg(messageLogContext.line).arg(messageLogContext.function).arg(messageLogContext.version);
    LogMessagePrivate(msgTypeString + message + logContextString);
}

QString LeafLogger::getFileName()
{
    return file.fileName();
}

LeafLogger::Garbo::Garbo()
{
    AsyncFileWriter::setFileDevice(&LeafLogger::file);
    AsyncFileWriter::setBuffer(&logBuffer,&logBufferMutex);
}

LeafLogger::Garbo::~Garbo(){
    logFileWriterController->deleteLater();
}

LeafLogger &LeafLogger::operator<<(const QString log)
{
    LogMessage(log);
    return *this;
}


//Initialize the global variables
QFile LeafLogger::file;
bool LeafLogger::isFileSetPath = false;
QMutex LeafLogger::consoleMutex;
LeafLogger::Garbo LeafLogger::garbo;
QQueue<QString> LeafLogger::logBuffer{};
QMutex LeafLogger::logBufferMutex;
AsyncFileWriter* LeafLogger::logFileWriterController = new AsyncFileWriter;

