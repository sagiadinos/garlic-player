#include "current_playing_media.h"

CurrentPlayingMedia::CurrentPlayingMedia(MediaManager *mm, QObject *parent) : QObject(parent)
{
    MyMediaManager = mm;
}

void CurrentPlayingMedia::insert(BaseMedia *SmilMedia)
{
    currently_playing_smilobjects.insert(SmilMedia);
    MyMediaManager->insertCurrentlyPlaying(SmilMedia->getSrc());
}

void CurrentPlayingMedia::remove(BaseMedia *SmilMedia)
{
    currently_playing_smilobjects.remove(SmilMedia);
    MyMediaManager->removeCurrentlyPlaying(SmilMedia->getSrc());
}

int CurrentPlayingMedia::count()
{
   return currently_playing_smilobjects.size();
}


BaseMedia *CurrentPlayingMedia::getFirstPlayingObject()
{
    QSet<BaseMedia *>::iterator i;
    if (currently_playing_smilobjects.size() > 0)
    {
        i = currently_playing_smilobjects.begin();
        return *i;
    }
    else
    {
        return Q_NULLPTR;
    }

}
