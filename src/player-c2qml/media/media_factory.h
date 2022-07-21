#ifndef MEDIA_FACTORY_H
#define MEDIA_FACTORY_H

#include <QQmlEngine>
#include <QObject>
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
        explicit MediaFactory(QQmlComponent *m, QString id, Launcher *l, QObject *parent = nullptr);
        PlayerBaseMedia *create(BaseMedia *media);

    private:
        PlayerBaseMedia *MyVideo, *MyImage, *MyBrush, *MyAudio, *MyWeb, *MyRefCommand;
        QQmlComponent *media_component;
        QString region_name;
        Launcher *MyLauncher;

};

#endif // MEDIA_FACTORY_H
