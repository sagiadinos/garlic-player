#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>
#include <mutex>

#include "configuration.h"
#include "logging_categories.h"

/**
 * @brief The Logger class
 * Thread-Safe C++11 Meyers' Singleton
 */
class Logger : public QObject
{
        Q_OBJECT
    public:
        const qint64 MAX_LOG_FILE_SIZE    = 10485760; // 10 MiB
        const qint64 MAX_LOG_FILE_NUMBER  = 20;
        static   Logger&                 getInstance();
                 void                    dispatchMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg);
                 QString                 createPlayLogEntry(QString start_time, QString content_id);
                 QString                 createEventLogMetaData(QString event_name, QStringList meta_data);
                 QFile                  *rotateFile(QFile *file);

    protected:
                 QScopedPointer<QFile>   qtdebug_log, debug_log, event_log, play_log;
                 void                    writeQtDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
                 void                    writeAppDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
                 void                    writeDebugLog(QFile *file, QtMsgType type, const QMessageLogContext &context, const QString &msg);
                 void                    writePlayLog(const QString &msg);
                 void                    writeEventLog(QtMsgType type, const QMessageLogContext &context, const QString &meta_data);
                 QString                 determineSeverity(QtMsgType type);
                 bool                    isSizeExeeds(QFile *file);

    private:
        explicit Logger(QObject *parent = nullptr);
                ~Logger()                          = default;
                 Logger(const Logger&)             = delete;
                 Logger & operator=(const Logger&) = delete;

          static Logger         *instance;
          static std::once_flag  initInstanceFlag;
          static void            initSingleton();
};

#endif // LOGGER_H
