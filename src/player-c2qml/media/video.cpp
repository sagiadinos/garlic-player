#include "video.h"

Video::Video(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);

    media_component = mc;
    // create signal as shitty QML has no signal for finished (EndOfFile)
    qml = "import QtQuick 2.12\n                             \
           import QtMultimedia 5.12\n                        \
                    Video {                                  \
                        id: "+getRegionId()+"_video;         \
                        orientation: 0;                      \
                        autoLoad: true;                      \
                        autoPlay: false;                      \
                        anchors.fill: parent;                \
                        signal qmlEndOfFile()\n \
                        onStopped: { if (status == MediaPlayer.EndOfMedia) {qmlEndOfFile()}\n} \
                     }\n";

    init();
}

Video::~Video()
{
}



void Video::loadMedia(BaseMedia *media, Region *reg)
{
    SmilMedia = media;
    region    = reg;
    loadInternal();
}

void Video::init()
{
    video_item.reset(createMediaItem(media_component, qml));
    if (!video_item.isNull())
        connect(video_item.data(), SIGNAL(qmlEndOfFile()), this, SLOT(doStopped()));
}

void Video::loadInternal()
{
    if (load(video_item.data()))
    {
        // to set Volume we need to cast
        TVideo *MyVideo = qobject_cast<TVideo *> (SmilMedia);
        float vol = determineVolume(MyVideo->getSoundLevel());
        video_item.data()->setProperty("volume", vol);
    }

}

void Video::play()
{
     // todo add support for pauseDisplay
    QMetaObject::invokeMethod(video_item.data(), "play");
    video_item.data()->setVisible(true);
    if (SmilMedia->getLogContentId() != "")
        setStartTime();
}


void Video::stop()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();

   QMetaObject::invokeMethod(video_item.data(), "stop");
   video_item.data()->setVisible(true);
}

void Video::resume()
{
    init();
    loadInternal();
    video_item.data()->setParentItem(parent_item);
    play();

    // look at pause explanation
//    video_item.data()->setVisible(true);
//    QMetaObject::invokeMethod(video_item.data(), "play");
}

void Video::pause()
{
    // numerous Android players cannot handle two videos at the same time. especially Philips Displays
    // this result in crashes or undefined reactions. Philips OS for example kills the old video hardcoded.
    // The Playlist will stop to continue and display the background screen
    // To prevent this we stop and deinitialize the video component in case of a pause and reload it when resume

    // QMetaObject::invokeMethod(video_item.data(), "pause");
    // todo add support for pauseDisplay
    // video_item.data()->setVisible(false);
    stop();
    video_item.reset();
}

void Video::setParentItem(QQuickItem *parent)
{
    parent_item = parent;
    video_item.data()->setParentItem(parent_item);
}

void Video::changeSize(int w, int h)
{
    QString smil_fit = SmilMedia->getFit().toLower();
    int fill_mode = NONE; // for do nothing
    if (smil_fit == "fill")
        fill_mode = Qt::IgnoreAspectRatio;
    else if (smil_fit == "meet")
        fill_mode = Qt::KeepAspectRatio;
    else if (smil_fit == "meetbest")
    {
//        if (loaded_image.width() >= w || loaded_image.height() > h)
//            fill_mode = Qt::KeepAspectRatio;
//       else
            fill_mode = NONE;
    }
    else if (smil_fit == "slice")
        fill_mode =  Qt::KeepAspectRatioByExpanding;

    video_item.data()->setProperty("fillMode",  fill_mode);
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
    if (video_item.data()->property("error").toInt() != 0)
    {
        QStringList list;
         list  << "resourceURI" << SmilMedia->getSrc()
              << "error"<< video_item.data()->property("error").toString()
              << "errorMessage" << video_item.data()->property("errorString").toString();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_PLAYBACK_ERROR", list);
    }

    SmilMedia->finishIntrinsicDuration();
}
