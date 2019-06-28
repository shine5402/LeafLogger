#include "../include/logfilewriter.h"

LogFileWriter::LogFileWriter(QObject *parent) : QObject(parent)
{

}

void LogFileWriter::setFileDevice(QFile* file)
{
    LogFileWriter::file = file;
}

void LogFileWriter::setBuffer(QQueue<QString>* buffer, QMutex* bufferMutex)
{
    LogFileWriter::buffer = buffer;
    LogFileWriter::bufferMutex = bufferMutex;
}

void LogFileWriter::doWork()
{
    QMutexLocker fileLocker(&fileMutex);
    if (!file)
        return;
    QTextStream writer(file);
    writer.setCodec(QTextCodec::codecForName("Utf8"));
    QMutexLocker bufferLocker(bufferMutex);
    for (int i = 0;i < buffer->count();i++) {
        writer << buffer->dequeue();
    };
}

QFile* LogFileWriter::file = nullptr;
QMutex LogFileWriter::fileMutex;
QQueue<QString>* LogFileWriter::buffer;
QMutex* LogFileWriter::bufferMutex;

void LogFileWriterController::handleFinished()
{

}

