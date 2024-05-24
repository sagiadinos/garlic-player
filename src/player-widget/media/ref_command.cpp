#include "ref_command.h"

PlayerRefCommand::PlayerRefCommand(MainConfiguration *config, QObject *parent) : PlayerBaseMedia(config, parent)
{

}

PlayerRefCommand::~PlayerRefCommand()
{

}

void PlayerRefCommand::loadMedia(BaseMedia *media, Region *reg)
{
    SmilMedia = media;
    region    = reg;
}

void PlayerRefCommand::restart()
{
    stop();
    play();
}

void PlayerRefCommand::play()
{
    QString path = SmilMedia->getLoadablePath();
    if (path.toLower() == "adapi:blankscreen")
    {
/*        if (MyMainConfiguration->getStandbyMode() == MyMainConfiguration->STANDBY_MODE_PARTIALLY)
            MyLauncher->toggleScreenActivity(false);
        else if (MyMainConfiguration->getStandbyMode() == MyMainConfiguration->STANDBY_MODE_DEEP)
            MyLauncher->activateDeepStandby(QString::number(SmilMedia->determineNextEndTime()));
  */
        if (MyMainConfiguration->getStandbyMode() == MyMainConfiguration->STANDBY_MODE_DEEP)
        {
            QString s = QString::number(SmilMedia->determineNextEndTime());
            qDebug() << s;
        }

        if (SmilMedia->getLogContentId() != "")
            setStartTime();
    }
    else
    {
         SmilMedia->finishedNotFound();
    }
}


void PlayerRefCommand::stop()
{
    // MyLauncher->toggleScreenActivity(true);
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void PlayerRefCommand::resume()
{

}

void PlayerRefCommand::pause()
{

}

void PlayerRefCommand::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}

QWidget *PlayerRefCommand::getView()
{
    return Q_NULLPTR;
}
