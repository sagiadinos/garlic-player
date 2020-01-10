#ifndef MEDIA_FACTORY_H
#define MEDIA_FACTORY_H

#include <QQmlEngine>
#include <QObject>
#include "media/image.h"
#include "media/web.h"
#include "media/video.h"
#include "media/audio.h"

class MediaFactory : public QObject
{
        Q_OBJECT
    public:
        explicit MediaFactory(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
        PlayerBaseMedia *initMedia(BaseMedia *media);

    protected:
        QScopedPointer<PlayerBaseMedia, QScopedPointerDeleteLater>  MyVideo, MyImage, MyAudio, MyWeb;

};

#endif // MEDIA_FACTORY_H
