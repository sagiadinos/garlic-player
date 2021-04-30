#include "media_factory.h"

MediaFactory::MediaFactory(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent) : QObject(parent)
{
    MyImage.reset(new Image(mc, r_id, lc, this));
    QQmlEngine::setObjectOwnership(MyImage.data(), QQmlEngine::CppOwnership);
    MyBrush.reset(new Brush(mc, r_id, lc, this));
    QQmlEngine::setObjectOwnership(MyBrush.data(), QQmlEngine::CppOwnership);
    MyVideo.reset(new Video(mc, r_id, lc, this));
    QQmlEngine::setObjectOwnership(MyVideo.data(), QQmlEngine::CppOwnership);
    MyAudio.reset(new Audio(mc, r_id, lc, this));
    QQmlEngine::setObjectOwnership(MyAudio.data(), QQmlEngine::CppOwnership);
    MyWeb.reset(new Web(mc, r_id, lc, this));
    QQmlEngine::setObjectOwnership(MyWeb.data(), QQmlEngine::CppOwnership);
    MyRefCommand.reset(new RefCommand(mc, r_id, lc, this));
    QQmlEngine::setObjectOwnership(MyRefCommand.data(), QQmlEngine::CppOwnership);
}

PlayerBaseMedia *MediaFactory::initMedia(BaseMedia *media)
{
    QString type   = media->objectName();
    if (type == "TImage")
    {
        MyImage.data()->init(media);
        return MyImage.data();
    }
    else if (type == "TBrush")
    {
        MyBrush.data()->init(media);
        return MyBrush.data();
    }
    else if (type == "TVideo")
    {
        MyVideo.data()->init(media);
        return MyVideo.data();
    }
    else if (type == "TAudio")
    {
        MyAudio.data()->init(media);
        return MyAudio.data();
    }
    else if (type == "TWeb")
    {
        MyWeb.data()->init(media);
        return MyWeb.data();
    }
    else if (type == "TWidget")
    {
        MyWeb.data()->init(media);
        return MyWeb.data();
    }
    else if (type == "TRefCommand")
    {
        MyRefCommand.data()->init(media);
        return MyRefCommand.data();
    }
    else
        return Q_NULLPTR;
}
