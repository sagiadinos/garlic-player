#include "media_factory.h"

MediaFactory::MediaFactory(QObject *parent) : QObject(parent)
{

}

BaseMedia *MediaFactory::createMedia(TMedia *media)
{
    QString type   = media->objectName();
    if (type == "TImage")
        return new Image(media);
    else if (type == "TVideo")
        return new Video(media);
    else if (type == "TAudio")
        return new Audio(media);
    else if (type == "TWeb")
        return new Web(media);
    else
        return Q_NULLPTR;
}
