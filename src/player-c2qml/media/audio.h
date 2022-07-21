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
        void                        play();
        void                        stop();
        void                        resume();
        void                        pause();
        void                        setParentItem(QQuickItem *parent);
        void                        changeSize(int w, int h);
        Q_INVOKABLE void stopDaShit();
    protected:
        QScopedPointer<QQuickItem, QScopedPointerDeleteLater>  audio_item;
        qreal                       determineVolume(QString percent);
        QString                     qml;
        QQmlComponent              *media_component;
    protected slots:
        void                        doStopped();
};

#endif // AUDIO_H
