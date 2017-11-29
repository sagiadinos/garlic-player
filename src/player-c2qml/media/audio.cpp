#include "audio.h"

Audio::Audio(QQmlComponent *mc, QString r_id, QObject *parent) : BaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
#ifdef SUPPORT_QTAV
    QString module = "import QtAV 1.7\n";
#else
    QString module = "import QtMultimedia 5.7\n";
#endif
    // FIXIT! Audio QML cannot be created
    // Qt Bug is reported https://bugreports.qt.io/browse/QTBUG-64763
    QString str("import QtQuick 2.7\n"+
                    module +
                "Video{\n \
                        id: "+getRegionId()+"_audio; \n \
                        autoPlay: true; \n \
                   }\n"
    );
    audio_item.reset(createMediaItem(mc, str));
    if (!audio_item.isNull())
        connect(audio_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
    else
        qCritical() << "audio item is Null";
}

Audio::~Audio()
{
    audio_item.reset();
}

void Audio::init(TMedia *media)
{
    MyMedia = media;
    QString source = MyMedia->getLoadablePath();
    if (isFileExists(source))
    {
        audio_item.data()->setProperty("source", "file:"+source);
        if (MyMedia->getLogContentId() != "")
            setStartTime();
    }
}

void Audio::deinit()
{
    if (MyMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
    audio_item.data()->setProperty("source", "");
}

void Audio::setParentItem(QQuickItem *parent)
{
    audio_item.data()->setParentItem(parent);
}

void Audio::finished()
{
    MyMedia->finishedSimpleDuration();
}

