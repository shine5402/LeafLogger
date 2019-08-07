#include "../include/asyncfilewriter.h"


AsyncFileWriterWorker::AsyncFileWriterWorker(QObject *parent) : QObject(parent)
{

}

QString AsyncFileWriterWorker::getFileName() const
{
    return file.fileName();
}

AsyncFileWriter::AsyncFileWriter(QObject* parent) : QObject (parent) {
    worker = new AsyncFileWriterWorker;
    worker->moveToThread(&workerThread);
    connect(this, &AsyncFileWriter::addToBuffer, worker, &AsyncFileWriterWorker::handleAddToBuffer);
    connect(this, &AsyncFileWriter::setFileName, worker, &AsyncFileWriterWorker::handleSetFileName);
    connect(worker, &AsyncFileWriterWorker::fileNotOpen, this, &AsyncFileWriter::fileNotOpen);
    workerThread.start();
}

QString AsyncFileWriter::getFileName() const
{
    return worker->getFileName();
}


void AsyncFileWriterWorker::doWork()
{
    QMutexLocker fileLocker(&fileMutex);
    if (!file.isOpen()){
        emit fileNotOpen();
        return;
    }
    QTextStream writer(&file);
    writer.setCodec(QTextCodec::codecForName("Utf8"));
    QMutexLocker bufferLocker(&bufferMutex);
    for (int i = 0;i < buffer.count();i++) {
        writer << buffer.dequeue();
    };
}

void AsyncFileWriterWorker::handleAddToBuffer(const QString& string)
{
    QMutexLocker locker(&bufferMutex);
    buffer.enqueue(string);
    doWork();
}

void AsyncFileWriterWorker::handleSetFileName(const QString& fileName)
{
    if (file.isOpen())
        file.close();
    file.setFileName(fileName);
    file.open(QIODevice::ReadWrite);
}

