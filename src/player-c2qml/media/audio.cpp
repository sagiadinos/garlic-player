#include "audio.h"

Audio::Audio(QQmlComponent *mc, QString r_id, Launcher *lc,  QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);
#ifdef SUPPORT_QTAV
    QString module = "import QtAV 1.7\n";
#else
    QString module = "import QtMultimedia 5.12\n";
#endif
    // FIXIT! Audio QML cannot be created
    // Qt Bug is reported https://bugreports.qt.io/browse/QTBUG-64763
    QString str = "import QtQuick 2.7\n"+
                    module +
                    "Video {                                 \
                        id: "+getRegionId()+"_video;         \
                        autoPlay: true;                      \
                    }\n";

    audio_item.reset(createMediaItem(mc, str));
    if (!audio_item.isNull())
        connect(audio_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
    else
        qCritical() << "audio item is Null";
}

Audio::~Audio()
{
}

void Audio::init(BaseMedia *media)
{
    SmilMedia = media;
    QString source = SmilMedia->getLoadablePath();
    if (isFileExists(source))
    {
        audio_item.data()->setProperty("source", "file:"+source);
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

void Audio::finished()
{
    SmilMedia->finishIntrinsicDuration();
}

