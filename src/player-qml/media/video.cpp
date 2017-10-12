#include "video.h"

Video::Video(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyVideo  = qobject_cast<TVideo *>(media);
}

Video::~Video()
{
}

void Video::init(QQmlComponent *mc)
{
    QString fill_mode = determineQmlFillMode(MyVideo->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";
#ifdef SUPPORT_QTAV
    QString module = "import  QtAV 1.7\n";
#else
    QString module = "import  QtMultimedia 5.7\n";
#endif
    QString str("import QtQuick 2.7\n"+
                    module +
                    "Video {\n " +
                        anchors+
                        "autoPlay: true;\n \
                        fillMode: VideoOutput."+fill_mode+";\n \
                        source: \"file:"+MyVideo->getLoadablePath()+"\";\n \
                   }\n"
    );
    video_item = createMediaItem(mc, str);
    connect(video_item, SIGNAL(stopped()), this, SLOT(finished()));
    connect(video_item, SIGNAL(destroyed(QObject*)), this, SLOT(doDestroy()));
}

void Video::setParentItem(QQuickItem *parent)
{
    video_item->setParentItem(parent);
}

void Video::finished()
{
    video_item->deleteLater();
}

void Video::doDestroy()
{
    MyVideo->finishedSimpleDuration();
}
