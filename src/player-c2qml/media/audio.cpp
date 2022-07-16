#include "audio.h"

Audio::Audio(QQmlComponent *mc, QString r_id, Launcher *lc,  QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);
    QString module = "import QtMultimedia 5.12\n";

    // FIXIT! Audio QML cannot be created
    // Qt Bug is reported https://bugreports.qt.io/browse/QTBUG-64763
    qml = "import QtQuick 2.7\n"+
                    module +
                    "Video {                                 \
                        id: "+getRegionId()+"_video;         \
                        autoPlay: true;                      \
                    }\n";

    audio_item.reset(createMediaItem(mc, qml));
    if (!audio_item.isNull())
        connect(audio_item.data(), SIGNAL(stopped()), this, SLOT(doStopped()));
    else
        qCritical() << "audio item is Null";
}

Audio::~Audio()
{
    audio_item.reset();
}

void Audio::init(BaseMedia *media, Region *reg)
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

void Audio::deinit()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();

    audio_item.data()->setProperty("source", "");
}

void Audio::setParentItem(QQuickItem *parent)
{
    audio_item.data()->setParentItem(parent);
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

    if (audio_item.data()->property("status").toString() == "7") // 7 means EndOfFile Shitty QML has no signal for finish
    {
       SmilMedia->finishIntrinsicDuration();
    }
}
