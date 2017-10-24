#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QLabel>
#include "mediadecoderwrapper.h"
#include "smilparser/audio.h"
#include "base_media.h"

class Audio : public BaseMedia
{
        Q_OBJECT
    public:
        explicit Audio(TMedia *media, QObject *parent = nullptr);
        ~Audio();
        void          init();
        QWidget      *getView();
        void          changeSize(int w, int h);
    protected:
        TAudio               *MyAudio;
        MediaDecoderWrapper   MediaDecoder;
    protected slots:
        void                  finished();
};

#endif // AUDIO_H
