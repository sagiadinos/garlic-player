#ifndef VIDEO_H
#define VIDEO_H

#include "mediaplayer_wrapper.h"
#include "mediawidget_wrapper.h"
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
        QScopedPointer<MediaWidgetWrapper>     VideoWidget;
        QScopedPointer<MediaPlayerWrapper, QScopedPointerDeleteLater>   MediaDecoder;
    protected slots:
        void                  finished();
};

#endif // VIDEO_H
