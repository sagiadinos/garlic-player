#ifndef AUDIO_H
#define AUDIO_H

#include "smilparser/media/video.h"
#include "player_base_media.h"

class Audio : public PlayerBaseMedia
{
        Q_OBJECT
    public:
        Audio(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
        ~Audio();
        void                        loadMedia(BaseMedia *media, Region *reg);
        void                        loadInternal();
        void                        play();
        void                        stop();
        void                        resume();
        void                        pause();
        void                        setParentItem(QQuickItem *parent);
        void                        changeSize(int w, int h);
    protected:
        QScopedPointer<QQuickItem, QScopedPointerDeleteLater>  media_item;
        qreal                       determineVolume(QString percent);
        QQmlComponent              *media_component;
        QQuickItem                 *parent_item;
        QTimer                     *KillTimer;
        QString                     qml;
        int                         last_position = 0;
        int                         getCurrentPosition();
    public slots:
        void                        doStopped();
        void                        checkForKill();
};

#endif // AUDIO_H
