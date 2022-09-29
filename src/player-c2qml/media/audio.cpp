#include "audio.h"

Audio::Audio(QQmlComponent *mc, QString r_id, Launcher *lc,  QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    media_component = mc;
    KillTimer = new QTimer(this);
    connect(KillTimer, SIGNAL(timeout()), this, SLOT(checkForKill()));

    // Audio QML cannot be created
    // Bug was reported at 26 Nov 2017 https://bugreports.qt.io/browse/QTBUG-64763
    // and updated on 2022-07-16

    qml = "import QtQuick 2.12\n                             \
           import QtMultimedia 5.12\n                        \
                   Video {                                   \
                        id: "+getRegionId()+"_audio;         \
                        autoLoad: true;                      \
                        autoPlay: false;                      \
            signal qmlEndOfFile()\n \
            onStopped: { if (status == MediaPlayer.EndOfMedia) {qmlEndOfFile()}\n} \
                     }\n";

}

Audio::~Audio()
{
}

void Audio::loadMedia(BaseMedia *media, Region *reg)
{
    SmilMedia = media;
    region    = reg;
    loadInternal();
}

void Audio::loadInternal()
{
    media_item.reset(createMediaItem(media_component, qml));
    if (!media_item.isNull())
        connect(media_item.data(), SIGNAL(qmlEndOfFile()), this, SLOT(doStopped()));

    if (load(media_item.data()))
    {
        // to set Volume we need to cast
        TAudio *MyAudio = qobject_cast<TAudio *> (SmilMedia);
        float vol = determineVolume(MyAudio->getSoundLevel());
        media_item.data()->setProperty("volume", vol);
    }
}

void Audio::play()
{
    // todo add support for pauseDisplay
   QMetaObject::invokeMethod(media_item.data(), "play");
   KillTimer->start(1000);
   media_item.data()->setVisible(true);
   if (SmilMedia->getLogContentId() != "")
       setStartTime();}

void Audio::stop()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();

   QMetaObject::invokeMethod(media_item.data(), "stop");
}

void Audio::resume()
{
    loadInternal();
    media_item.data()->setParentItem(parent_item);
    play();

    // look at pause explanation
//    video_item.data()->setVisible(true);
//    QMetaObject::invokeMethod(video_item.data(), "play");

}

void Audio::pause()
{
    // numerous Android players cannot handle two videos at the same time. especially Philips Displays
    // this result in crashes or undefined reactions. Philips OS for example kills the old video hardcoded.
    // The Playlist will stop to continue and display the background screen
    // To prevent this we stop and deinitialize the video component in case of a pause and reload it when resume

    // QMetaObject::invokeMethod(video_item.data(), "pause");
    // todo add support for pauseDisplay
    // video_item.data()->setVisible(false);
    stop();
    media_item.reset();}

void Audio::setParentItem(QQuickItem *parent)
{
    parent_item = parent;
    media_item.data()->setParentItem(parent);
}

void Audio::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}

qreal Audio::determineVolume(QString percent)
{
    qreal vol = 0;
    if (percent.endsWith('%'))
        vol = percent.midRef(0, percent.length()-1).toFloat();

    return vol / (float) 100;
}

int Audio::getCurrentPosition()
{
    return media_item.data()->property("position").toInt();
}

void Audio::doStopped()
{
    if (media_item.data()->property("error").toInt() != 0)
    {
        QStringList list;
         list  << "resourceURI" << SmilMedia->getSrc()
              << "error"<< media_item.data()->property("error").toString()
              << "errorMessage" << media_item.data()->property("errorString").toString();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_PLAYBACK_ERROR", list);
    }
    SmilMedia->finishIntrinsicDuration();
}

void Audio::checkForKill()
{
   int current = getCurrentPosition();
   if (current > last_position)
   {
        last_position = current;
   }
   else
   {
        doStopped();
   }
}

