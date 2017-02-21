#include "wgt.h"

TWgt::TWgt()
{

}

void TWgt::setDurationTimerBeforePlay()
{
    if (load() && (hasDurAttribute() || end_timer->isActive()))
    {
        if (!is_resumed)
            emit startedMedia(parent_container, this);
    }
    else // set a duration otherwise it runs in a recursion stack overflow when no dur set or load is not complete
        setInternalDefaultDur();
    return;
}

void TWgt::play()
{
    status = _playing;
    return;
}

void TWgt::pause()
{
    status = _paused;
    return;
}


void TWgt::stop()
{
    delete browser;
    status = _stopped;
    loaded = false;
    return;
}

bool TWgt::loadMedia()
{
    browser       = new QWebEngineView();
    browser->load(QUrl(MyFileManager->getLoadablePath(src)));

    browser->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    qDebug() << getID() << QTime::currentTime().toString()  << "maybe ;) loaded: " << src;
    return true;
}

void TWgt::setAttributes()
{
    setBaseMediaAttributes();
    return;
}
