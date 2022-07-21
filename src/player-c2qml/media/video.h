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
        const int TILE               = 4;
        const int TILE_V             = 5;
        const int NONE               = 6;

        Video(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
        ~Video();
        void                        loadMedia(BaseMedia *media, Region *reg);
        void                        play();
        void                        stop();
        void                        resume();
        void                        pause();
        void                        setParentItem(QQuickItem *parent);
        void                        changeSize(int w, int h);
    protected:
        QScopedPointer<QQuickItem, QScopedPointerDeleteLater>  video_item;
        qreal                       determineVolume(QString percent);
        QString                     qml;
        QQmlComponent              *media_component;
      public slots:
        void                        doStopped();
};

#endif // VIDEO_H
