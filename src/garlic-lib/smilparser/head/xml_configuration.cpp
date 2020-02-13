#include "xml_configuration.h"

SmilHead::XMLConfiguration::XMLConfiguration(MainConfiguration *config, QObject *parent) : BaseManager(config, parent)
{
    XMLDownloader        = new Downloader(MyConfiguration, this);
    connect(XMLDownloader, SIGNAL(succeed(TNetworkAccess *)), SLOT(doSucceed(TNetworkAccess *)));
}

void SmilHead::XMLConfiguration::processFromUrl(QUrl config_url)
{
    XMLDownloader->processFile(config_url, MyConfiguration->getPaths("cache")+"configuration.xml");
}

void SmilHead::XMLConfiguration::processFromLocalFile(QString file_path)
{
    if (!loadDocument(file_path))
        return;
    parse();
}

bool SmilHead::XMLConfiguration::loadDocument(QString file_path)
{
    QFile       file(file_path);
    QStringList list;
    list << "instanceId" << "display:0"
         << "resourceURI" << file_path
         << "contentLength" << QString::number(file.size())
         << "lastModifiedTime" << QFileInfo(file).lastModified().toString(Qt::ISODate);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical(MediaControl) << Logger::getInstance().createEventLogMetaData("CONFIGURATION_OPEN_ERROR", list);
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
        qCritical(MediaControl) << Logger::getInstance().createEventLogMetaData("CONFIGURATION_XML_ERROR", list);
        file.close();
        return false;
    }
    file.close();
    qInfo(MediaControl) << Logger::getInstance().createEventLogMetaData("CONFIGURATION_LOADED", list);
    return true;
}


void SmilHead::XMLConfiguration::doSucceed(TNetworkAccess *network)
{
    Q_UNUSED(network);
    QString path = MyConfiguration->getPaths("cache")+"configuration.xml";
    renameDownloadedFile(path);
    processFromLocalFile(path);
}

void SmilHead::XMLConfiguration::parse()
{
    QDomNodeList node_list = document.elementsByTagName("prop");
    QString attr_name      = "";
    QString attr_value     = "";
    for(int i = 0; i < node_list.size(); i++)
    {
        attr_name  = getAttribute(node_list.at(i).toElement(), "name");
        attr_value = getAttribute(node_list.at(i).toElement(), "value");
        if (attr_value == "")
            continue;

        if (attr_name == "info.playerName")
        {
            MyConfiguration->setPlayerName(attr_value);
        }
        else if (attr_name == "content.serverUrl")
        {
            MyConfiguration->setIndexUri(attr_value);
            MyConfiguration->setUserConfigByKey("index_uri", attr_value);
        }
        else if (attr_name == "display.brightness")
        {
            MyConfiguration->setUserConfigByKey("display/brightness", attr_value);
        }
        else if (attr_name == "hardware.videoOut.0.rotation")
        {
            MyConfiguration->setUserConfigByKey("display/rotation", attr_value);
        }
        else if (attr_name == "audio.soundlevel")
        {
            MyConfiguration->setUserConfigByKey("audio/soundlevel", attr_value);
        }
        else if (attr_name == "hardware.audioOut.0.masterSoundLevel")
        {
            MyConfiguration->setUserConfigByKey("audio/soundlevel", attr_value);
        }
        else if (attr_name == "time.tzDescription")
        {
            MyConfiguration->setUserConfigByKey("time/tzDescription", attr_value);
        }
        else if (attr_name == "time.tzCode")
        {
            MyConfiguration->setUserConfigByKey("time/tzCode", attr_value);
        }
        else if (attr_name == "time.autoUpdate.protocol")
        {
            MyConfiguration->setUserConfigByKey("time/protocol", attr_value);
        }
        else if (attr_name == "time.autoUpdate.server")
        {
            MyConfiguration->setUserConfigByKey("time/server", attr_value);
        }
        else if (attr_name == "task.scheduledReboot.days")
        {
            MyConfiguration->setUserConfigByKey("schedule/reboot_days", attr_value);
        }
        else if (attr_name == "schedule.reboot.days")
        {
            MyConfiguration->setUserConfigByKey("schedule/reboot_days", attr_value);
        }
        else if (attr_name == "schedule.reboot.time")
        {
            MyConfiguration->setUserConfigByKey("schedule/reboot_time", attr_value);
        }
        else if (attr_name == "task.scheduledReboot.time")
        {
            MyConfiguration->setUserConfigByKey("schedule/reboot_time", attr_value);
        }
    }
    emit finishedConfiguration();
}


QString SmilHead::XMLConfiguration::getAttribute(QDomElement element, QString attr_name)
{
    QString ret = "";
    if (element.hasAttribute(attr_name))
    {
        ret = element.attribute(attr_name);
    }
    return ret;
}
