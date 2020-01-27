#include "unknown.h"

Unknown::Unknown(TContainer *pc, QObject *parent) : BaseMedia(parent)
{
    parent_container = pc;
    setObjectName("Unknown");
    is_media = false;

}

void Unknown::setDurationTimerBeforePlay()
{
    qDebug(Develop) << src << " Unknown tag";
    initInternalTimer();
}

void Unknown::setAttributes()
{
    parseBaseMediaAttributes();
}
