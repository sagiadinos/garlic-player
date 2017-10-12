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
        static   BaseMedia *createMedia(TMedia *media);
    signals:

    public slots:
};

#endif // MEDIA_FACTORY_H
