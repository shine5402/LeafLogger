#ifndef LOGFILEWRITER_H
#define LOGFILEWRITER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMutex>
#include <QThread>
#include <QQueue>

class LogFileWriter : public QObject
{
    Q_OBJECT
public:
    explicit LogFileWriter(QObject *parent = nullptr);
    static void setFileDevice(QFile* file);
    static void setBuffer(QQueue<QString>* buffer,QMutex* bufferMutex);

signals:
    void finished();
public slots:
    void doWork();
private:
    static QFile* file;
    static QMutex fileMutex;
    static QQueue<QString>* buffer;
    static QMutex* bufferMutex;
};
class LogFileWriterController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    LogFileWriterController(QObject* parent = nullptr) : QObject (parent) {
        LogFileWriter *worker = new LogFileWriter;
        worker->moveToThread(&workerThread);
        connect(this, &LogFileWriterController::bufferReady, worker, &LogFileWriter::doWork);
        connect(worker,&LogFileWriter::finished,this,&LogFileWriterController::handleFinished);
        workerThread.start();
    }
    ~LogFileWriterController() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleFinished();
signals:
    void bufferReady();
};

#endif // LOGFILEWRITER_H
