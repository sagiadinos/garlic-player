#include "logger.h"

Logger* Logger::instance= nullptr;
std::once_flag Logger::initInstanceFlag;

Logger::Logger(QObject *parent) : QObject(parent)
{
    qtdebug_log.reset(new QFile(TConfiguration::getLogDir() + "qtdebug.log"));
    qtdebug_log.data()->open(QFile::Append | QFile::Text);

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
    if (QString(context.category) == "EventLog" ||
            QString(context.category) == "SmilParser" ||
            QString(context.category) == "ContentManager" ||
            QString(context.category) == "MediaPlayer" ||
            QString(context.category) == "MediaControl" ||
            QString(context.category) == "System" ||
            QString(context.category) == "TimeService")
    {
        writeEventLog(type, context, msg);
    }
    else if (QString(context.category).mid(0,3) == "qt.")
    {
        writeQtDebugLog(type, context, msg); // get all qt log rubbish here not poisoning debug_log
    }
    else if (QString(context.category) == "PlayLog")
    {
        writePlayLog(msg);
    }
    else
    {
        writeAppDebugLog(type, context, msg);
    }
}

QString Logger::createPlayLogEntry(QString start_time, QString content_id)
{
    return "<contentPlayed><contentId>"+content_id+"</contentId><startTime>"+start_time+"</startTime><endTime>" +QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"</endTime></contentPlayed>";
}

QString Logger::createEventLogMetaData(QString event_name, QStringList meta_data)
{
    QString xml = "<eventName>"+event_name+"</eventName>";
    for (int i = 0; i < meta_data.size(); i = i+2)
    {
        xml += "<metadata><meta name=\""+meta_data.at(i)+"\" content=\""+meta_data.at(i+1)+"\"/></metadata>";
    }
    return xml.replace("/", "");
}

void Logger::writeAppDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    writeDebugLog(debug_log.data(), type, context, msg);
}

void Logger::writeQtDebugLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    writeDebugLog(qtdebug_log.data(), type, context, msg);
}

void Logger::writeDebugLog(QFile *file, QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " "
        << determineSeverity(type) << " "
        << context.category << " "
        << context.file << " "
        << context.line << " "
        << msg
        << endl;
    out.flush();
}


void Logger::writePlayLog(const QString &msg)
{
    QTextStream out(play_log.data());
    out << msg << endl;
    out.flush();
}

void Logger::writeEventLog(QtMsgType type, const QMessageLogContext &context, const QString &meta_data)
{
    QTextStream out(event_log.data());
    out << "<event>"
        << "<eventType>" << determineSeverity(type) << "</eventType>"
        << "<eventTime>" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "</eventTime>"
        << "<eventSource>" << context.category << "</eventSource>"
        << meta_data
        << "</event>"
        << endl;
    out.flush();
}

QString Logger::determineSeverity(QtMsgType type)
{
    switch (type)
    {
        case QtDebugMsg:
            return "debug";
        case QtInfoMsg:
            return "informational";
        case QtWarningMsg:
            return "warning";
        case QtCriticalMsg:
            return "error";
        case QtFatalMsg:
            return "critical";
    }
    return "UNKNOWN";
}
