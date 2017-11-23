#include "logger.h"

Logger* Logger::instance= nullptr;
std::once_flag Logger::initInstanceFlag;

Logger::Logger(QObject *parent) : QObject(parent)
{
    debug_log.reset(new QFile(TConfiguration::getLogDir() + "debug.log"));
    debug_log.data()->open(QFile::Append | QFile::Text);
    event_log.reset(new QFile(TConfiguration::getLogDir() + "event_log.xml"));
    event_log.data()->open(QFile::Append | QFile::Text);
    play_log.reset(new QFile(TConfiguration::getLogDir() + "play_log.xml"));
    play_log.data()->open(QFile::Append | QFile::Text);
}

void Logger::initSingleton()
{
    instance = new Logger;
}

Logger& Logger::getInstance()
{
    std::call_once(initInstanceFlag, &Logger::initSingleton);
    return *instance;
}

void Logger::dispatchMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (context.category == "PlayLog")
    {
        writePlayLog(type, context, msg);
    }
    else if (context.category == "EventLog")
    {
        writeEventLog(type, context, msg);
    }
    else
    {
        writeDebugLog(type, context, msg);
    }
}

void Logger::writeDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(debug_log.data());

    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ")
        << determineSeverity(type)
        << context.file
        << context.line
        << msg
        << endl;
    out.flush();    // Clear the buffered data
}

void Logger::writePlayLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // dummy
}

void Logger::writeEventLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // dummy
}

QString Logger::determineSeverity(QtMsgType type)
{
    switch (type)
    {
        case QtDebugMsg:
            return "DEBUG ";
        case QtInfoMsg:
            return "INFO ";
        case QtWarningMsg:
            return "WARNING ";
        case QtCriticalMsg:
            return "ERROR ";
        case QtFatalMsg:
            return "FATAL ";
    }
    return "UNKNOWN";
}
