#include "unknown.h"

Unknown::Unknown(TContainer *pc, Files::MediaManager *mm, MainConfiguration *config, QObject *parent) : BaseMedia(mm, config, parent)
{
    parent_container = pc;
    setObjectName("Unknown");
    is_media = false;

}

void Unknown::registerInMediaManager()
{
}

void Unknown::prepareDurationTimerBeforePlay()
{
    qDebug(Develop) << src << " Unknown tag";
    skipElement();
}

void Unknown::setAttributes()
{
    parseBaseMediaAttributes();
}
