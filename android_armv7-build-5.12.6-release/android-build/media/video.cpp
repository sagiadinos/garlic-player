#include "video.h"

Video::Video(QQmlComponent *mc, QString r_id, QObject *parent) : PlayerBaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
#ifdef SUPPORT_QTAV
    QString module = "import QtAV 1.7\n";
#else
    QString module = "import QtMultimedia 5.7\n";
#endif
    qml = "import QtQuick 2.7\n"+
                    module +
                    "Video { \
                        id: "+getRegionId()+"_video; \
                        orientation: 0; \
                        anchors.fill: parent; \
                        autoPlay: true; \
                        property var video_fill_mode: \"\";  \
                   }\n";
    media_component = mc;
    video_item.reset(createMediaItem(media_component, qml));
}

Video::~Video()
{
    video_item.reset();
}

void Video::init(BaseMedia *media)
{
    MyMedia = media;
    if (load(video_item.data()))
    {
        // cannot be connected once in constructor due to again Android/QMultimedia problems
        // see @ method finished
        connect(video_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
        // to set Volume we need to cast
        TVideo *MyVideo = qobject_cast<TVideo *> (media);
        float vol = determineVolume(MyVideo->getSoundLevel());
        video_item.data()->setProperty("volume", vol);
        video_item.data()->setProperty("fillMode", determineFillMode(MyMedia->getFit()));
        if (MyMedia->getLogContentId() != "")
            setStartTime();
    }
}

void Video::deinit()
{
    if (MyMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();

    video_item.data()->setProperty("source", "");
}

void Video::setParentItem(QQuickItem *parent)
{
    video_item.data()->setParentItem(parent);
}

int Video::determineFillMode(QString smil_fit)
{
    // FIXME Look if this code can set in Base Class
    if (smil_fit == "fill")
        return STRETCH;
    else if (smil_fit == "meet")
        return PRESERVEASPECTCROP;
    else if (smil_fit == "meetbest")
        return PRESERVEASPECTFIT;
    else
        return STRETCH;
}

qreal Video::determineVolume(QString percent)
{
    qreal vol = 0;
    if (percent.endsWith('%'))
        vol = percent.mid(0, percent.length()-1).toFloat();

    return vol / (float) 100;
}

void Video::finished()
{
    // must be disconected otherwise QMultimedia/Android will stop immediately when new video will be load
    // so only first Video will shown
    disconnect(video_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
    if (video_item.data()->property("error").toInt() != 0)
    {
        QStringList list;
         list  << "resourceURI" << MyMedia->getSrc()
              << "error"<< video_item.data()->property("error").toString()
              << "errorMessage" << video_item.data()->property("errorString").toString();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_PLAYBACK_ERROR", list);
    }
    MyMedia->finishedSimpleDuration();

}
