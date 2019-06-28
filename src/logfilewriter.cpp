#include "../include/logfilewriter.h"

LogFileWriter::LogFileWriter(QObject *parent) : QObject(parent)
{

}

void LogFileWriter::setFileDevice(QFile* file)
{
    LogFileWriter::file = file;
}

void LogFileWriter::doWork(const QString& log)
{
    QMutexLocker locker(&fileMutex);
    if (!file)
        return;
    QTextStream writer(file);
    writer.setCodec(QTextCodec::codecForName("Utf8"));
    writer << log;
}

QFile* LogFileWriter::file = nullptr;
QMutex LogFileWriter::fileMutex;

void LogFileWriterController::handleFinished()
{
    workerThread.quit();
}

