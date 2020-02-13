#include "unknown.h"

Unknown::Unknown(TContainer *pc, QObject *parent) : BaseMedia(parent)
{
    parent_container = pc;
    setObjectName("Unknown");
    is_media = false;

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
