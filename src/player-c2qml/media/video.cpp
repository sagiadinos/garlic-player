#include "video.h"

Video::Video(QQmlComponent *mc, QString r_id, QObject *parent) : BaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
#ifdef SUPPORT_QTAV
    QString module = "import  QtAV 1.7\n";
#else
    QString module = "import  QtMultimedia 5.7\n";
#endif
    QString str("import QtQuick 2.7\n"+
                    module +
                    "Video { \
                        id: "+getRegionId()+"_video; \
                        anchors.fill: parent; \
                        autoPlay: true; \
                        property var video_fill_mode: \"\";  \
                        source: \"file:\";  \
                        onStatusChanged:  \
                        {    \
                            parent.parent.fitVideo(this, video_fill_mode); \
                        }  \
                   }\n"
    );
// onSourceChanged: gc(); \n
    video_item.reset(createMediaItem(mc, str));
    connect(video_item.data(), SIGNAL(stopped()), this, SLOT(finished()));
}

Video::~Video()
{
    video_item.reset();
}

void Video::init(TMedia *media)
{
    MyVideo = qobject_cast<TVideo *>(media);
    video_item.data()->setProperty("video_fill_mode", MyVideo->getFit());
    QString source = "file:"+MyVideo->getLoadablePath();
    video_item.data()->setProperty("source", source);
}

void Video::deinit()
{
    video_item.data()->setProperty("source", "");
}

void Video::setParentItem(QQuickItem *parent)
{
    video_item.data()->setParentItem(parent);
}

void Video::finished()
{
    MyVideo->finishedSimpleDuration();
}

