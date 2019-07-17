#ifndef AUDIO_H
#define AUDIO_H

#include <QLabel>
#include "mediaplayer_wrapper.h"
#include "smilparser/audio.h"
#include "base_media.h"

class Audio : public BaseMedia
{
        Q_OBJECT
    public:
        explicit Audio(QObject *parent = nullptr);
        ~Audio();
        void          init(TMedia *media);
        void          deinit();
        QWidget      *getView();
        void          changeSize(int w, int h);
    protected:
        QScopedPointer<MediaPlayerWrapper, QScopedPointerDeleteLater>   MediaDecoder;
    protected slots:
        void                  finished();
};

#endif // AUDIO_H
