#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>
#include <mutex>

#include "configuration.h"

/**
 * @brief The Logger class
 * Thread-Safe C++11 Meyers' Singleton
 */
class Logger : public QObject
{
        Q_OBJECT
    public:
        static   Logger& getInstance();
                 void    dispatchMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    protected:
        QScopedPointer<QFile>   debug_log;
        QScopedPointer<QFile>   event_log;
        QScopedPointer<QFile>   play_log;
        void                    writeDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
        void                    writePlayLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
        void                    writeEventLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
        QString                 determineSeverity(QtMsgType type);

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
