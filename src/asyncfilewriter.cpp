#include "../include/asyncfilewriter.h"

AsyncFileWriterWorker::AsyncFileWriterWorker(QObject *parent) : QObject(parent)
{

}

void AsyncFileWriter::setFileDevice(QFile* file)
{
    AsyncFileWriterWorker::file = file;
}

void AsyncFileWriter::setBuffer(QQueue<QString>* buffer, QMutex* bufferMutex)
{
    AsyncFileWriterWorker::buffer = buffer;
    AsyncFileWriterWorker::bufferMutex = bufferMutex;
}

void AsyncFileWriterWorker::doWork()
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

QFile* AsyncFileWriterWorker::file = nullptr;
QMutex AsyncFileWriterWorker::fileMutex;
QQueue<QString>* AsyncFileWriterWorker::buffer;
QMutex* AsyncFileWriterWorker::bufferMutex;

void AsyncFileWriter::handleFinished()
{

}

