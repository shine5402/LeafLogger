#ifndef LOGFILEWRITER_H
#define LOGFILEWRITER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMutex>
#include <QThread>
#include <QQueue>
class AsyncFileWriter;
class AsyncFileWriterWorker : public QObject
{
    friend AsyncFileWriter;
    Q_OBJECT
signals:
    void finished();
private slots:
    void doWork();
private:
    explicit AsyncFileWriterWorker(QObject *parent = nullptr);
    static QFile* file;
    static QMutex fileMutex;
    static QQueue<QString>* buffer;
    static QMutex* bufferMutex;

};
class AsyncFileWriter : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    AsyncFileWriter(QObject* parent = nullptr) : QObject (parent) {
        AsyncFileWriterWorker *worker = new AsyncFileWriterWorker;
        worker->moveToThread(&workerThread);
        connect(this, &AsyncFileWriter::bufferReady, worker, &AsyncFileWriterWorker::doWork);
        connect(worker,&AsyncFileWriterWorker::finished,this,&AsyncFileWriter::handleFinished);
        workerThread.start();
    }
    static void setBuffer(QQueue<QString>* buffer,QMutex* bufferMutex);
    static void setFileDevice(QFile* file);
    ~AsyncFileWriter() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleFinished();
signals:
    void bufferReady();
};

#endif // LOGFILEWRITER_H
