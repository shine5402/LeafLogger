#ifndef LOGFILEWRITER_H
#define LOGFILEWRITER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMutex>
#include <QThread>
#include <QQueue>
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
    QString getFileName() const;
public slots:
signals:
    void addToBuffer(const QString& string);
    void setFileName(const QString& fileName);//BUG:将FileName直接写入
    void fileNotOpen();
private:
    AsyncFileWriterWorker *worker = nullptr;
};

class AsyncFileWriterWorker : public QObject
{
    friend AsyncFileWriter;
    Q_OBJECT
signals:
    void finished();
    void fileNotOpen();
private slots:
    void doWork();
    void handleAddToBuffer(const QString& string);
    void handleSetFileName(const QString& fileName);
private:
    explicit AsyncFileWriterWorker(QObject *parent = nullptr);
    QFile file;//把所需基本设施迁移至Writer
    QMutex fileMutex;
    QQueue<QString> buffer;
    QMutex bufferMutex;
    QString getFileName() const;

};
#endif // LOGFILEWRITER_H
