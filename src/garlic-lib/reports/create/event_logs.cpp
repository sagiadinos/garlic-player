#include "event_logs.h"

Reporting::CreateEventLogs::CreateEventLogs(TConfiguration *config,
                                            QObject *parent) : Reporting::CreateBase(config, parent)
{

}

void Reporting::CreateEventLogs::process(QString file_path)
{
    init();
    mergeXml(file_path);
}

void Reporting::CreateEventLogs::mergeXml(QString file_path)
{
    QFile file;
    QDomDocument player_event_log;
    file.setFileName(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning(Develop) << "cannot open file" << file_path;
        file.close();
        return;
    }

    QByteArray xml        = "<playerEventLog>" + file.readAll() + "</playerEventLog>";
    QString error_message = "";
    int error_line, error_column = 0;
    if (!player_event_log.setContent(xml, &error_message, &error_line, &error_column))
    {
        qWarning(Develop) << "cannot read xml" << file_path << error_message
                          << "Line: " << QString::number(error_line)
                          << "Column: " << QString::number(error_column);
        file.close();
        return;
    }
    file.close();
    player.appendChild(player_event_log);
    file.remove();
}
