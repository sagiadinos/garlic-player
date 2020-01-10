#ifndef VIDEO_H
#define VIDEO_H

#include "smilparser/media/video.h"
#include "player_base_media.h"

class Video : public PlayerBaseMedia
{
        Q_OBJECT
    public:
        const int STRETCH            = 0;
        const int PRESERVEASPECTFIT  = 1;
        const int PRESERVEASPECTCROP = 2;
        Video(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
        ~Video();
        void                        init(BaseMedia *media);
        void                        deinit();
        void                        setParentItem(QQuickItem *parent);
    protected:
        QScopedPointer<QQuickItem>  video_item;
        int                         determineFillMode(QString smil_fit);
        qreal                       determineVolume(QString percent);
        QString                     qml;
        QQmlComponent              *media_component;
    protected slots:
        void                        finished();
};

#endif // VIDEO_H
