#ifndef AUDIO_H
#define AUDIO_H

#include "smilparser/media/video.h"
#include "base_media.h"

class Audio : public BaseMedia
{
        Q_OBJECT
    public:
        Audio(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
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
