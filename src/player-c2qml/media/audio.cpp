#include "audio.h"

Audio::Audio(QQmlComponent *mc, QString r_id, Launcher *lc,  QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);

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

   audio_item.reset(createMediaItem(mc, qml));
   connect(audio_item.data(), SIGNAL(qmlEndOfFile()), this, SLOT(doStopped()));
}

Audio::~Audio()
{
    audio_item.reset();
}

void Audio::loadMedia(BaseMedia *media, Region *reg)
{
    SmilMedia = media;
    region    = reg;
    if (load(audio_item.data()))
    {
        // to set Volume we need to cast
         TAudio *MyAudio = qobject_cast<TAudio *> (media);
         float vol = determineVolume(MyAudio->getSoundLevel());
         audio_item.data()->setProperty("volume", vol);
         if (SmilMedia->getLogContentId() != "")
             setStartTime();
    }
}

void Audio::play()
{
     if (SmilMedia->getLogContentId() != "")
         setStartTime();

     QMetaObject::invokeMethod(audio_item.data(), "play");
}

void Audio::stop()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();

    QMetaObject::invokeMethod(audio_item.data(), "stop");
}

void Audio::resume()
{
    QMetaObject::invokeMethod(audio_item.data(), "play");
}

void Audio::pause()
{
    QMetaObject::invokeMethod(audio_item.data(), "pause");
}

void Audio::setParentItem(QQuickItem *parent)
{
    audio_item.data()->setParentItem(parent);
}

void Audio::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}

void Audio::stopDaShit()
{
    SmilMedia->finishIntrinsicDuration();
}

qreal Audio::determineVolume(QString percent)
{
    qreal vol = 0;
    if (percent.endsWith('%'))
        vol = percent.midRef(0, percent.length()-1).toFloat();

    return vol / (float) 100;
}

void Audio::doStopped()
{
    if (audio_item.data()->property("error").toInt() != 0)
    {
        QStringList list;
         list  << "resourceURI" << SmilMedia->getSrc()
              << "error"<< audio_item.data()->property("error").toString()
              << "errorMessage" << audio_item.data()->property("errorString").toString();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_PLAYBACK_ERROR", list);
    }

   SmilMedia->finishIntrinsicDuration();
}
