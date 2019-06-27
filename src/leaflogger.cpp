#include "../include/leaflogger.h"
#include<stdio.h>

void LeafLogger::LogMessage(const QString log)
{
    QMutexLocker locker(&mutex);
    QDateTime current_date_time =QDateTime::currentDateTime();
    if (!isFileSetPath) {
        QString log_text = QString(u8"[%1] %2\n").arg(current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz")).arg("Warning: the path of file for log is not specificed, the log will not be saved to any file.");
        fprintf(stderr,log_text.toUtf8().data());
    }

    QString log_text = QString(u8"[%1] %2\n").arg(current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz")).arg(log);
        fprintf(stderr,log_text.toUtf8().data());
    if (isFileSetPath){
        QTextStream writer(&file);
        writer.setCodec(QTextCodec::codecForName("Utf8"));
        writer << log_text;
    }
}

void LeafLogger::LogMessage(QMetaObject *metaObject, const QString log)
{
   LogMessage(QString("[Log from %1] %2").arg(metaObject->className()).arg(log));
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
LeafLogger LeafLogger::getLogger()
{
    return LeafLogger();
}

void LeafLogger::LogSysInfo()
{
    LeafLogger::LogMessage("======  System Info  ======");
    LeafLogger::LogMessage(QString(u8"bootUniqueId: %1").arg(QString(QSysInfo::bootUniqueId())));
    LeafLogger::LogMessage(QString(u8"buildAbi: %1").arg(QSysInfo::buildAbi()));
    LeafLogger::LogMessage(QString(u8"currentCpuArchitecture: %1").arg(QSysInfo::currentCpuArchitecture()));
    LeafLogger::LogMessage(QString(u8"machineUniqueId: %1").arg(QString(QSysInfo::machineUniqueId())));
    LeafLogger::LogMessage(QString(u8"machineHostName: %1").arg(QSysInfo::machineHostName()));
    LeafLogger::LogMessage(QString(u8"prettyProductName: %1").arg(QSysInfo::prettyProductName()));
    LeafLogger::LogMessage("======  System Info End  ======");
}

void LeafLogger::LogInit()
{
    setFilePathWithTime();
    LogSysInfo();
}

void LeafLogger::messageHandler(QtMsgType msgType, const QMessageLogContext& messageLogContext, const QString& message)
{
    QString msgTypeString;
    switch (msgType) {
    case QtDebugMsg :
        msgTypeString = "[Debug]";
        break;
    case QtInfoMsg :
        msgTypeString = "[Info]";
        break;
    case QtWarningMsg :
        msgTypeString = "[Warning]";
        break;
    case QtCriticalMsg :
        msgTypeString = "[Critical]";
        break;
    case QtFatalMsg :
        msgTypeString = "[Fatal]";
    }
    QString logContextString = QString("\n++++ category: %1 | file: %2 line: %3\n++++ function : %4 | version : %5").arg(messageLogContext.category).arg(messageLogContext.file).arg(messageLogContext.line).arg(messageLogContext.function).arg(messageLogContext.version);
    LogMessage(msgTypeString + message + logContextString);
}

QString LeafLogger::getFileName()
{
    return file.fileName();
}

LeafLogger &LeafLogger::operator<<(const QString log)
{
    LogMessage(log);
    return *this;
}

QFile LeafLogger::file;
bool LeafLogger::isFileSetPath = false;
QMutex LeafLogger::mutex;


