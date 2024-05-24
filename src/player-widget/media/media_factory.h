#ifndef MEDIA_FACTORY_H
#define MEDIA_FACTORY_H

#include <QObject>
#include "player_base_media.h"

using namespace MediaParser;

class MediaFactory : public QObject
{
        Q_OBJECT
    public:
        explicit MediaFactory(QObject *parent = nullptr);
        PlayerBaseMedia *create(MainConfiguration *config, BaseMedia *media);

    signals:

    public slots:

    protected:
        PlayerBaseMedia  *MyVideo, *MyImage, *MyBrush, *MyAudio, *MyWeb, *MyRefCommand;


};

#endif // MEDIA_FACTORY_H
