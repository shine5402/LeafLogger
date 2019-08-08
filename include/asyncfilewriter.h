#ifndef LOGFILEWRITER_H
#define LOGFILEWRITER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMutex>
#include <QThread>
#include <QQueue>
#include <QDebug>
#include <QTimer>

class AsyncFileWriterWorker;

class AsyncFileWriter : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    AsyncFileWriter(QObject* parent = nullptr);

    ~AsyncFileWriter() {
        workerThread.quit();
        workerThread.wait();
    }

public slots:
    QString fileName() const;
    void setFileName(const QString& fileName, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    void addToBuffer(const QString& string);
signals:
    void fileNotOpen();
    void bufferReady();
private:
    AsyncFileWriterWorker *worker = nullptr;
    QFile file;
    QMutex fileMutex;
    QQueue<QString> buffer;
    QMutex bufferMutex;
};

class AsyncFileWriterWorker : public QObject
{
    friend AsyncFileWriter;
    Q_OBJECT
signals:
    void finished();
    void fileNotOpen();
protected:
    void timerEvent(QTimerEvent*) override;
private slots:
    void doWork();
private:
    explicit AsyncFileWriterWorker(QFile* fileDevice, QMutex* fileMutex, QQueue<QString>* buffer,QMutex* bufferMutex,QObject *parent = nullptr);
    QFile* file = nullptr;
    QMutex* fileMutex = nullptr;
    QQueue<QString>* buffer = nullptr;
    QMutex* bufferMutex = nullptr;

};
#endif // LOGFILEWRITER_H
