#include "audio.h"

Audio::Audio(QQmlComponent *mc, QString r_id, QObject *parent) : BaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
#ifdef SUPPORT_QTAV
    QString module = "import  QtAV 1.7\n";
#else
    QString module = "import  QtMultimedia 5.7\n";
#endif
    QString str("import QtQuick 2.7\n"+
                    module +
                    "Audio {\n \
                        id: "+getRegionId()+"_audio;\n \
                        autoPlay: true;\n \
                   }\n"
    );
// onSourceChanged: gc(); \n
    audio_item.reset(createMediaItem(mc, str));
    connect(audio_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
}

Audio::~Audio()
{
    audio_item.reset();
}

void Audio::init(TMedia *media)
{
    MyAudio = qobject_cast<TAudio *>(media);
    QString source = "file:"+MyAudio->getLoadablePath();
    audio_item.data()->setProperty("source", source);
}

void Audio::deinit()
{
    audio_item.data()->setProperty("source", "");
}

void Audio::setParentItem(QQuickItem *parent)
{
    audio_item.data()->setParentItem(parent);
}

void Audio::finished()
{
    MyAudio->finishedSimpleDuration();
}

