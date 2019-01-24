#include "../include/leaflogger.h"
void LeafLogger::LogMessage(const QString log)
{
    QMutexLocker locker(&mutex);
    QDateTime current_date_time =QDateTime::currentDateTime();
    if (!isFileSetPath) {
        QString log_text = QString(u8"[%1] %2\n").arg(current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz")).arg(QStringLiteral("警告：日志文件IO设备没有被指定路径，日志将不会被记录到文件中。"));
        qDebug() << log_text;
    }

    QString log_text = QString(u8"[%1] %2\n").arg(current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz")).arg(log);
    qDebug() << log_text;
    if (isFileSetPath){
        QTextStream writer(&file);
        writer.setCodec(QTextCodec::codecForName("Utf8"));
        writer << log_text;
    }
}

void LeafLogger::LogMessage(const wchar_t *log)
{
    LogMessage(QString::fromWCharArray(log));
}
void LeafLogger::setFilePath(QString Path)
{
    if (file.isOpen())
        file.close();
    file.setFileName(Path);
    file.open(QIODevice::ReadWrite);
    //writer.setDevice(file);
    isFileSetPath = true;
}

void LeafLogger::setFilePathWithTime()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QDir* dir = new QDir;
    if (!dir->exists("./log")){
        dir->mkdir("log");
    }
    delete dir;
    setFilePath(QString("./log/log%1.txt").arg(current_date_time.toString("yyyyMMddhhmmsszzz")));
}
LeafLogger LeafLogger::getLogger()
{
    return LeafLogger();
}

void LeafLogger::LogSysInfo()
{
    LeafLogger::LogMessage(QString(u8"启动唯一标识符（仅Linux与Apple）：%1").arg(QString(QSysInfo::bootUniqueId())));
    LeafLogger::LogMessage(QString(u8"当前程序ABI：%1").arg(QSysInfo::buildAbi()));
    LeafLogger::LogMessage(QString(u8"当前系统架构：%1").arg(QSysInfo::currentCpuArchitecture()));
    LeafLogger::LogMessage(QString(u8"机器唯一标识符：%1").arg(QString(QSysInfo::machineUniqueId())));
    LeafLogger::LogMessage(QString(u8"机器主机名：%1").arg(QSysInfo::machineHostName()));
    LeafLogger::LogMessage(QString(u8"操作系统：%1").arg(QSysInfo::prettyProductName()));
}

void LeafLogger::LogInit()
{
    setFilePathWithTime();
    LogSysInfo();
}

LeafLogger &LeafLogger::operator<<(const QString log)
{
    LogMessage(log);
    return *this;
}

LeafLogger &LeafLogger::operator<<(const wchar_t *log)
{
    LogMessage(QString::fromWCharArray(log));
    return *this;
}

void LeafLogger::LogMessage(const QString log, const char* func)
{
    QString log_with_func = QString(u8"函数：%1：%2").arg(func).arg(log);
    LogMessage(log_with_func);
}

void LeafLogger::LogMessage(const QString log, const char* func, const char* file, int line)
{
    QString log_with_func_line = QString("（%4:%2）函数：%1：%3").arg(func).arg(line).arg(log).arg(file);
    LogMessage(log_with_func_line);
}
QFile LeafLogger::file;
bool LeafLogger::isFileSetPath = false;
//QTextStream LeafLogger::writer = QTextStream(file);
QMutex LeafLogger::mutex;


