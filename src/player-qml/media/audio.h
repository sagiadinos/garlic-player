#ifndef AUDIO_H
#define AUDIO_H

#include "smilparser/audio.h"
#include "base_media.h"

class Audio : public BaseMedia
{
        Q_OBJECT
    public:
        explicit Audio(TMedia *media, QObject *parent = nullptr);
        ~Audio();
        void          init(QQmlComponent *mc);
        void          setParentItem(QQuickItem *parent);
     protected:
        TAudio           *MyAudio;
        QQuickItem       *audio_item = Q_NULLPTR;
    protected slots:
        void                  finished();
        void                  doDestroy();
};

#endif // AUDIO_H
