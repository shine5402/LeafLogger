#ifndef LOGFILEWRITER_H
#define LOGFILEWRITER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMutex>
#include <QThread>

class LogFileWriter : public QObject
{
    Q_OBJECT
public:
    explicit LogFileWriter(QObject *parent = nullptr);
    static void setFileDevice(QFile* file);

signals:
    void finished();
public slots:
    void doWork(const QString& log);
private:
    static QFile* file;
    static QMutex fileMutex;
};
class LogFileWriterController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    LogFileWriterController(QObject* parent = nullptr) : QObject (parent) {
        LogFileWriter *worker = new LogFileWriter;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &LogFileWriterController::operate, worker, &LogFileWriter::doWork);
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
    void operate(const QString &);
};

#endif // LOGFILEWRITER_H
