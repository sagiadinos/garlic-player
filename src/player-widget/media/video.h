#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>

#include "mediadecoderwrapper.h"
#include "mediaviewwrapper.h"
#include "smilparser/video.h"
#include "base_media.h"

class Video : public BaseMedia
{
        Q_OBJECT
    public:
        Video(TMedia *media, QObject *parent = nullptr);
        ~Video();
        void              init();
        void              changeSize(int w, int h);
        QWidget          *getView();
    protected:
        TVideo    *MyVideo;
        MediaViewWrapper     *VideoWidget;
        MediaDecoderWrapper   MediaDecoder;
    protected slots:
        void                  finished();
};

#endif // VIDEO_H
