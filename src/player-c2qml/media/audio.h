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
        TAudio                     *MyAudio;
        QScopedPointer<QQuickItem>  audio_item;
    protected slots:
        void                        finished();
};

#endif // AUDIO_H
