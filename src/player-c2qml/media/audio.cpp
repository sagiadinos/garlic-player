#include "audio.h"

Audio::Audio(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    // should stay a raw pointer cause it belongs to Parser
    MyAudio = qobject_cast<TAudio *>(media);
}

Audio::~Audio()
{
}

void Audio::init(QQmlComponent *mc)
{
#ifdef SUPPORT_QTAV
    QString module = "import  QtAV 1.7\n";
#else
    QString module = "import  QtMultimedia 5.7\n";
#endif
    QString str("import QtQuick 2.7\n"+
                    module +
                    "Audio {\n \
                        autoPlay: true;\n \
                        source: \"file:"+MyAudio->getLoadablePath()+"\";\n \
                   }\n"
    );
    audio_item.reset(createMediaItem(mc, str));
    connect(audio_item.data(), SIGNAL(stopped()), this, SLOT(finishedAudio()));
}

void Audio::setParentItem(QQuickItem *parent)
{
    audio_item.data()->setParentItem(parent);
}

void Audio::finished()
{
    MyAudio->finishedSimpleDuration();
}

