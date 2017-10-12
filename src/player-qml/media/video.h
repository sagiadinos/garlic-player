#ifndef VIDEO_H
#define VIDEO_H

#include "smilparser/video.h"
#include "base_media.h"

class Video : public BaseMedia
{
        Q_OBJECT
    public:
        Video(TMedia *media, QObject *parent = nullptr);
        ~Video();
        void              init(QQmlComponent *mc);
        void              setParentItem(QQuickItem *parent);
    protected:
        TVideo               *MyVideo;
        QQuickItem           *video_item = Q_NULLPTR;
    protected slots:
        void                  finished();
        void                  doDestroy();
};

#endif // VIDEO_H
