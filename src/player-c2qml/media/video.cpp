#include "video.h"

Video::Video(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);

#ifdef SUPPORT_QTAV
    QString module = "import QtAV 1.7\n";
#else
    QString module = "import QtMultimedia 5.12\n";
#endif
    qml = "import QtQuick 2.12\n " +
                    module +
                    "Video {                                 \
                        id: "+getRegionId()+"_video;         \
                        orientation: 0;                      \
                        autoLoad: true;                      \
                        autoPlay: true;                      \
                        anchors.fill: parent;                \
                    }\n";

    media_component = mc;
    video_item.reset(createMediaItem(media_component, qml));
    connect(video_item.data(), SIGNAL(stopped()), this, SLOT(doStopped()));
}

Video::~Video()
{
    video_item.reset();
}

void Video::init(BaseMedia *media)
{
    SmilMedia = media;
    if (load(video_item.data()))
    {
        // cannot be connected once in constructor due to again Android/QMultimedia problems
        // see @ method finished
        // connect(video_item.data(), SIGNAL(stopped()), this, SLOT(doStopped()));

        // to set Volume we need to cast
        TVideo *MyVideo = qobject_cast<TVideo *> (media);
        float vol = determineVolume(MyVideo->getSoundLevel());
        video_item.data()->setProperty("volume", vol);
        video_item.data()->setProperty("fillMode", determineFillMode(SmilMedia->getFit().toLower()));
        if (SmilMedia->getLogContentId() != "")
            setStartTime();
    }
}

void Video::deinit()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();

    video_item.data()->setProperty("source", "");
}

void Video::setParentItem(QQuickItem *parent)
{
    video_item.data()->setParentItem(parent);
}

int Video::determineFillMode(QString smil_fit)
{
    if (smil_fit == "fill")
        return STRETCH;
    else if (smil_fit == "meet")
        return PRESERVEASPECTFIT;
    else if (smil_fit == "meetbest")
        return PRESERVEASPECTFIT;
    else if (smil_fit == "slice")
        return PRESERVEASPECTCROP;
    else
        return NONE;
}

qreal Video::determineVolume(QString percent)
{
    qreal vol = 0;
    if (percent.endsWith('%'))
        vol = percent.midRef(0, percent.length()-1).toFloat();

    return vol / (float) 100;
}


void Video::doStopped()
{
    // deprecated? 17.01.2021
    // must be disconected otherwise QMultimedia/Android will stop immediately when new video will be load
    // so only first Video will shown
    // disconnect(video_item.data(), SIGNAL(qmlSignal(QString)), this, SLOT(doStopped(QString)));

    if (video_item.data()->property("error").toInt() != 0)
    {
        QStringList list;
         list  << "resourceURI" << SmilMedia->getSrc()
              << "error"<< video_item.data()->property("error").toString()
              << "errorMessage" << video_item.data()->property("errorString").toString();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_PLAYBACK_ERROR", list);
    }

    if (video_item.data()->property("status").toString() == "7") // 7 means EndOfFile Shitty QML has no signal for finish
    {
        SmilMedia->finishIntrinsicDuration();
    }
}
