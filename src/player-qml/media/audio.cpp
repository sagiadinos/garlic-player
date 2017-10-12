#include "audio.h"

Audio::Audio(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyAudio  = qobject_cast<TAudio *>(media);
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
    audio_item = createMediaItem(mc, str);
    connect(audio_item, SIGNAL(stopped()), this, SLOT(finishedAudio()));
    connect(audio_item, SIGNAL(destroyed(QObject*)), this, SLOT(doDestroy()));
}

void Audio::setParentItem(QQuickItem *parent)
{
    audio_item->setParentItem(parent);
}

void Audio::finished()
{
    audio_item->deleteLater();
}

void Audio::doDestroy()
{
    MyAudio->finishedSimpleDuration();
}

