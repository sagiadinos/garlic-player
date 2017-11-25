#ifndef VIDEO_H
#define VIDEO_H

#include "mediadecoderwrapper.h"
#include "mediaviewwrapper.h"
#include "smilparser/video.h"
#include "base_media.h"

class Video : public BaseMedia
{
        Q_OBJECT
    public:
        Video(QObject *parent = nullptr);
        ~Video();
        void              init(TMedia *media);
        void              deinit();
        void              changeSize(int w, int h);
        QWidget          *getView();
    protected:
        QScopedPointer<MediaViewWrapper>     VideoWidget;
        QScopedPointer<MediaDecoderWrapper, QScopedPointerDeleteLater>   MediaDecoder;
    protected slots:
        void                  finished();
};

#endif // VIDEO_H
