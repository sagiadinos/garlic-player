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
        void                        init(BaseMedia *media);
        void                        deinit();
        void                        setParentItem(QQuickItem *parent);
    protected:
        QScopedPointer<QQuickItem>  audio_item;
    protected slots:
        void                        finished();
};

#endif // AUDIO_H
