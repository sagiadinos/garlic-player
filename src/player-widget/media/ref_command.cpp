#include "ref_command.h"

PlayerRefCommand::PlayerRefCommand(QObject *parent) : PlayerBaseMedia(parent)
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
     //   MyLauncher->toggleScreenActivity(false);
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
