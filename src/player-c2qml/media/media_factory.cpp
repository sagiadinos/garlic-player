#include "media_factory.h"

MediaFactory::MediaFactory(QQmlComponent *m, QString id, Launcher *l, QObject *parent) : QObject(parent)
{
    media_component   = m;
    region_name = id;
    MyLauncher   = l;

}

PlayerBaseMedia *MediaFactory::create(BaseMedia *media)
{
    QString type   = media->objectName();
    if (type == "TImage")
    {
        MyImage = new Image(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyImage, QQmlEngine::CppOwnership);
        return MyImage;
    }
    else if (type == "TBrush")
    {
        MyBrush = new Brush(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyBrush, QQmlEngine::CppOwnership);
        return MyBrush;
    }
    else if (type == "TVideo")
    {
        MyVideo = new Video(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyVideo, QQmlEngine::CppOwnership);
        return MyVideo;
    }
    else if (type == "TAudio")
    {
        MyAudio = new Audio(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyAudio, QQmlEngine::CppOwnership);
        return MyAudio;
    }
    else if (type == "TWeb")
    {
        MyWeb = new Web(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyWeb, QQmlEngine::CppOwnership);
        return MyWeb;
    }
    else if (type == "TWidget")
    {
        MyWeb = new Web(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyWeb, QQmlEngine::CppOwnership);
        return MyWeb;
    }
    else if (type == "TRefCommand")
    {
        MyRefCommand = new RefCommand(media_component, region_name, MyLauncher, this);
        QQmlEngine::setObjectOwnership(MyRefCommand, QQmlEngine::CppOwnership);
        return MyRefCommand;
    }
    else
        return Q_NULLPTR;
}
