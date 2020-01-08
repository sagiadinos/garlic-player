#include "task_scheduler.h"

SmilHead::TaskScheduler::TaskScheduler(TConfiguration *config, QObject *parent) : BaseManager(parent)
{
    MyConfiguration           = config;
    TaskFileDownloader        = new Downloader(MyConfiguration, this);
    connect(TaskFileDownloader, SIGNAL(succeed(TNetworkAccess *)), SLOT(doSucceedParse(TNetworkAccess *)));
}


void SmilHead::TaskScheduler::init(QString task_scheduler_path)
{
    task_scheduler_url = QUrl(task_scheduler_path);
    TaskFileDownloader->processFile(task_scheduler_url, MyConfiguration->getPaths("cache")+"task_scheduler.xml");
}


bool SmilHead::TaskScheduler::loadDocument(QString file_path)
{
    QFile       file(file_path);
    QStringList list;
    list << "instanceId" << "display:0"
         << "resourceURI" << file_path
         << "contentLength" << QString::number(file.size())
         << "lastModifiedTime" << QFileInfo(file).lastModified().toString(Qt::ISODate);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical(MediaControl) << Logger::getInstance().createEventLogMetaData("TASK_SCHEDULER_OPEN_ERROR", list);
        file.close();
        return false;
    }
    QString error_message = "";
    int error_line, error_column = 0;
    if (!document.setContent(&file, &error_message, &error_line, &error_column))
    {
        list << "errorMessage" << error_message
             << "errorLine" << QString::number(error_line)
             << "errorColumn" << QString::number(error_column);
        qCritical(MediaControl) << Logger::getInstance().createEventLogMetaData("TASK_SCHEDULER_XML_ERROR", list);
        file.close();
        return false;
    }
    file.close();
    qInfo(MediaControl) << Logger::getInstance().createEventLogMetaData("TASK_SCHEDULER_LOADED", list);
    return true;
}


/**
 * @brief SmilHead::TaskScheduler::doSucceed
 * @param network
 */
void SmilHead::TaskScheduler::doSucceedParse(TNetworkAccess *network)
{
    Q_UNUSED(network);
    renameDownloadedFile(MyConfiguration->getPaths("cache")+"task_scheduler.xml");
    if (!loadDocument(MyConfiguration->getPaths("cache")+"task_scheduler.xml"))
        return;

    QDomNodeList node_list = document.elementsByTagName("timedTaskList");
    if (node_list.size() == 0)
        return;

    QDomNodeList childs = node_list.at(0).toElement().childNodes();
    for(int i = 0; i < childs.size(); i++)
    {
        if (childs.item(i).toElement().tagName() == "updateSettings")
        {
            parseUpdateSetting(childs.item(i).toElement());
        }
        else if (childs.item(i).toElement().tagName() == "firmwareUpdate")
        {
            parseFirmwareUpdate(childs.item(i).toElement());
        }
        else if (childs.item(i).toElement().tagName() == "shutdownPlayer")
        {
            parseShutdownPlayer(childs.item(i).toElement());
        }
    }
    return;
}

void SmilHead::TaskScheduler::emitApplyConfiguration()
{
    qInfo(TaskExecution) << Logger::getInstance().createTaskExecutionLogEntry(MyUpdateSetting.task_id, "completed");
    emit applyConfiguration();
}

void SmilHead::TaskScheduler::emitUpdateFirware()
{
    emit updateFirmware();
}

void SmilHead::TaskScheduler::emitShutdownPlayer()
{
    emit shutdownPlayer();
}

void SmilHead::TaskScheduler::parseFirmwareUpdate(QDomElement element)
{
    if (element.hasAttribute("id"))
    {
        MyFirmwareUpdate.task_id = element.attribute("id");
    }

    if (hasUsedTaskId(MyFirmwareUpdate.task_id, "firmware_update_task_id"))
        return;

    QDomNodeList childs = element.childNodes();

    for (int i = 0; i < childs.length(); i++)
    {
        QDomElement el = childs.at(i).toElement();
        if (el.tagName() == "sourceURI")
        {
            MyFirmwareUpdate.source_uri = QUrl(el.text());
        }
        else if (el.tagName() == "contentType")
        {
            MyFirmwareUpdate.version = el.text();
        }
        else if (el.tagName() == "contentType")
        {
            MyFirmwareUpdate.content_type = el.text();
        }
        else if (el.tagName() == "contentLength")
        {
            MyFirmwareUpdate.content_length = el.text().toInt();
        }
        else if (el.tagName() == "contentChecksum")
        {
            MyFirmwareUpdate.content_checksum = el.text();
        }
        else if (el.tagName() == "checksumMethod")
        {
            MyFirmwareUpdate.checksum_method = el.text();
        }
    }
    return;

}


void SmilHead::TaskScheduler::parseUpdateSetting(QDomElement element)
{
    if (element.hasAttribute("id"))
    {
        MyUpdateSetting.task_id = element.attribute("id");
    }

    if (hasUsedTaskId(MyUpdateSetting.task_id, "update_settings_task_id"))
        return;

    QDomNodeList childs = element.childNodes();

    for (int i = 0; i < childs.length(); i++)
    {
        QDomElement el = childs.at(i).toElement();
        if (el.tagName() == "sourceURI")
        {
            MyUpdateSetting.source_uri = QUrl(el.text());
        }
        else if (el.tagName() == "contentType")
        {
            MyUpdateSetting.content_type = el.text();
        }
        else if (el.tagName() == "contentLength")
        {
            MyUpdateSetting.content_length = el.text().toInt();
        }
        else if (el.tagName() == "contentChecksum")
        {
            MyUpdateSetting.content_checksum = el.text();
        }
        else if (el.tagName() == "checksumMethod")
        {
            MyUpdateSetting.checksum_method = el.text();
        }
    }
    MyXMLConfiguration.reset(new SmilHead::XMLConfiguration(MyConfiguration, this));
    connect(MyXMLConfiguration.data(), SIGNAL(finishedConfiguration()), SLOT(emitApplyConfiguration()));

    MyXMLConfiguration.data()->init(MyUpdateSetting.source_uri);
    return;
}

void SmilHead::TaskScheduler::parseShutdownPlayer(QDomElement element)
{
    if (element.hasAttribute("id"))
    {
        MyShutdownPlayer.task_id = element.attribute("id");
    }

    if (hasUsedTaskId(MyShutdownPlayer.task_id, "shudown_player_task_id"))
        return;

}

bool SmilHead::TaskScheduler::hasUsedTaskId(QString task_id, QString task_name)
{
    if (MyConfiguration->getUserConfigByKey(task_name) == task_id)
    {
        // do not apply a formely used update
        return true;
    }

    MyConfiguration->setUserConfigByKey(task_name, task_id);

    return false;
}



