#ifndef MEDIA_FACTORY_H
#define MEDIA_FACTORY_H

#include "media/image.h"
#include "media/brush.h"
#include "media/web.h"
#include "media/video.h"
#include "media/audio.h"
#include "media/ref_command.h"

class MediaFactory : public QObject
{
        Q_OBJECT
    public:
        explicit MediaFactory(QObject *parent = nullptr);
        PlayerBaseMedia *initMedia(BaseMedia *media);

    signals:

    public slots:

    protected:
        QScopedPointer<PlayerBaseMedia, QScopedPointerDeleteLater>  MyVideo, MyImage, MyBrush, MyAudio, MyWeb, MyRefCommand;


};

#endif // MEDIA_FACTORY_H
