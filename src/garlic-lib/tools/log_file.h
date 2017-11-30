#ifndef LOGFILE_H
#define LOGFILE_H

#include <QDir>
#include <QFileInfo>
#include <QScopedPointer>
#include <QTextStream>

class LogFile : public QObject
{
        Q_OBJECT
    public:
        explicit LogFile(QString file_path, QObject *parent = nullptr);
        void     setMaxSize(qint64 size);
        void     setMaxNumber(qint64 number);
        void     write(QString data);
        void     rotateFile();
    protected:
        const qint64 MAX_LOG_FILE_SIZE    = 1048576; // 1 MiB
        const qint64 MAX_LOG_FILE_NUMBER  = 30;
        QString                     buffer;
        bool                        is_rotated = false;
        QTextStream                 out;
        QScopedPointer<QFile>       log_file;
        QScopedPointer<QFileInfo>   log_file_info;
        qint64                      max_size   = MAX_LOG_FILE_SIZE;
        qint64                      max_number = MAX_LOG_FILE_NUMBER;
        bool                        isSizeExeeds();
};

#endif // LOGFILE_H
