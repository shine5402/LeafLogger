#include "../include/asyncfilewriter.h"


AsyncFileWriterWorker::AsyncFileWriterWorker(QFile* fileDevice, QMutex* fileMutex, QQueue<QString>* buffer, QMutex* bufferMutex, QObject *parent) : QObject(parent),file(fileDevice),fileMutex(fileMutex),buffer(buffer),bufferMutex(bufferMutex)
{
    startTimer(50);
}


AsyncFileWriter::AsyncFileWriter(QObject* parent) : QObject (parent) {
    worker = new AsyncFileWriterWorker(&file,&fileMutex,&buffer,&bufferMutex,this);
    worker->moveToThread(&workerThread);
    connect(this,&AsyncFileWriter::bufferReady,worker,&AsyncFileWriterWorker::doWork);
    connect(worker, &AsyncFileWriterWorker::fileNotOpen, this, &AsyncFileWriter::fileNotOpen);
    workerThread.start();
}

QString AsyncFileWriter::fileName() const
{
    return file.fileName();
}

void AsyncFileWriter::setFileName(const QString& fileName, QIODevice::OpenMode openMode)
{
    if (file.isOpen())
        file.close();
    file.setFileName(fileName);
    file.open(openMode);
}

void AsyncFileWriter::addToBuffer(const QString& string)
{
    QMutexLocker locker(&bufferMutex);
    buffer.enqueue(string);
    emit bufferReady();
}

void AsyncFileWriterWorker::timerEvent(QTimerEvent*)
{
    doWork();
}

void AsyncFileWriterWorker::doWork()
{
    QMutexLocker fileLocker(fileMutex);
    if (!file->isOpen()){
        emit fileNotOpen();
        qDebug() << "AsyncFileWriter cannot use the file I/O device, because it doesn't open.";
        return;
    }
    QMutexLocker bufferLocker(bufferMutex);
    if (buffer->empty())
        return;
    QTextStream writer;
    writer.setCodec(QTextCodec::codecForName("Utf8"));
    writer.setDevice(file);
    for (int i = 0;i < buffer->count();i++) {
        writer << buffer->dequeue();
    };
}
