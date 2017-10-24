#include "video.h"

Video::Video(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    // should stay a raw pointer cause it belongs to Parser
    MyVideo = qobject_cast<TVideo *>(media);
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
    QString codec = "videoCodecPriority: [\"MediaCodec\", \"FFmpeg\"]; \n \
                    opengl: true; \n \"";
#else
    QString module = "import  QtMultimedia 5.7\n";
    QString codec = "";
#endif
    QString str("import QtQuick 2.7\n"+
                    module +
                    "Video {\n " +
                        anchors+
                        "autoPlay: true;\n \
                        fillMode: VideoOutput."+fill_mode+";\n \
                        source: \"file:"+MyVideo->getLoadablePath()+"\";\n \
                        onSourceChanged: gc(); \n \
                   }\n"
    );
    video_item.reset(createMediaItem(mc, str));
    connect(video_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
}

void Video::setParentItem(QQuickItem *parent)
{
    video_item.data()->setParentItem(parent);
}

void Video::finished()
{
    MyVideo->finishedSimpleDuration();
}

