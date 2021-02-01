#include "player_configuration.h"

PlayerConfiguration::PlayerConfiguration(MainConfiguration *mc, QObject *parent) : QObject(parent)
{
    MyMainConfiguration = mc;
}

void PlayerConfiguration::determineInitConfigValues()
{
    determineUuid();
    MyMainConfiguration->determinePlayerName();
    if (!launcher_version.isEmpty())
        MyMainConfiguration->setAdditionalVersion("L" + launcher_version);
    determineSmilIndexUri();
    MyMainConfiguration->determineUserAgent();

    QApplication::setApplicationName(MyMainConfiguration->getAppName());
    QApplication::setApplicationVersion(MyMainConfiguration->getVersion());
    QApplication::setApplicationDisplayName(MyMainConfiguration->getAppName());

    QDir dir(".");
    MyMainConfiguration->determineBasePath(dir.absolutePath());
    MyMainConfiguration->createDirectories();

}

void PlayerConfiguration::setHasLauncher(bool value)
{
    has_launcher = value;
}

void PlayerConfiguration::setUuidFromLauncher(QString value)
{
    launcher_uuid = value;
}

void PlayerConfiguration::setVersionFromLauncher(QString value)
{
    launcher_version = value;
}


void PlayerConfiguration::setSmilIndexUriFromLauncher(QString value)
{
    launcher_smil_index_uri = value;
    if (value != MyMainConfiguration->getIndexUri())
    {
        MyMainConfiguration->setIndexUri(value);
    }
}

void PlayerConfiguration::determineUuid()
{
    if (has_launcher && launcher_uuid != MyMainConfiguration->getUuid())
    {
        MyMainConfiguration->setUuid(launcher_uuid);
        return;
    }

    if(MyMainConfiguration->getUuid().isEmpty())
    {
        MyMainConfiguration->setUuid(MyMainConfiguration->createUuid());

    }
}

void PlayerConfiguration::determineSmilIndexUri()
{
    if (has_launcher && launcher_smil_index_uri != MyMainConfiguration->getIndexUri())
    {
        MyMainConfiguration->setIndexUri(launcher_smil_index_uri);
    }


}

void PlayerConfiguration::determinePlayerName()
{

}

