#ifndef AUDIO_H
#define AUDIO_H

#include <QLabel>
#include "mediaplayer_wrapper.h"
#include "smilparser/media/audio.h"
#include "player_base_media.h"

class PlayerAudio : public PlayerBaseMedia
{
        Q_OBJECT
    public:
        explicit PlayerAudio(QObject *parent = nullptr);
        ~PlayerAudio();
        void          loadMedia(BaseMedia *media, Region *reg);
        void          play();
        void          stop();
        void          resume();
        void          pause();
        QWidget      *getView();
        void          changeSize(int w, int h);
    protected:
        QScopedPointer<MediaPlayerWrapper, QScopedPointerDeleteLater>   MediaDecoder;
    protected slots:
        void                  finished();
};

#endif // AUDIO_H
