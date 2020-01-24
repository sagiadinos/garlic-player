#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>
#include <mutex>

#include "main_configuration.h"
#include "log_file.h"
#include "logging_categories.h"

/**
 * @brief The Logger class
 * Thread-Safe C++11 Meyers' Singleton
 */
class Logger : public QObject
{
        Q_OBJECT
    public:
        static   Logger&                  getInstance();
                 void                     dispatchMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg);
                 QString                  createPlayLogEntry(QString start_time, QString content_id);
                 QString                  createTaskExecutionLogEntry(QString task_id, QString type);
                 QString                  createEventLogMetaData(QString event_name, QStringList meta_data);
                 void                     rotateLog(QString log_name);
                 QString                  getCurrentIsoDateTime();
    protected:
                 QScopedPointer<LogFile>  qtdebug_log, debug_log, play_log, event_log, task_execution_log;
                 QString                  collectDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
                 QString                  collectEventLog(QtMsgType type, const QMessageLogContext &context, const QString &meta_data);
                 QString                  determineSeverity(QtMsgType type);

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
