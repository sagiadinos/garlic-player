#ifndef MEDIA_FACTORY_H
#define MEDIA_FACTORY_H

#include <QObject>
#include "media/image.h"
#include "media/web.h"
#include "media/video.h"
#include "media/audio.h"

class MediaFactory : public QObject
{
        Q_OBJECT
    public:
        explicit MediaFactory(QObject *parent = nullptr);
        BaseMedia *initMedia(TMedia *media);

    signals:

    public slots:

    protected:
        QScopedPointer<BaseMedia, QScopedPointerDeleteLater>  MyVideo, MyImage, MyAudio, MyWeb;


};

#endif // MEDIA_FACTORY_H
