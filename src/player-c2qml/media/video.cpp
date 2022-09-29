#include "video.h"

Video::Video(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent) : Audio(mc, r_id, lc, parent)
{
    // create signal as shitty QML has no signal for finished (EndOfFile)
    qml = "import QtQuick 2.12\n                             \
           import QtMultimedia 5.12\n                        \
                    Video {                                  \
                        id: "+getRegionId()+"_video;         \
                        orientation: 0;                      \
                        autoLoad: true;                      \
                        autoPlay: false;                      \
                        anchors.fill: parent;                \
                        signal qmlEndOfFile()\n \
                        onStopped: { if (status == MediaPlayer.EndOfMedia) {qmlEndOfFile()}\n} \
                     }\n";

}

Video::~Video()
{
}

void Video::loadInternal()
{
    if (load(media_item.data()))
    {
        // to set Volume we need to cast
        TVideo *MyVideo = qobject_cast<TVideo *> (SmilMedia);
        float vol = determineVolume(MyVideo->getSoundLevel());
        media_item.data()->setProperty("volume", vol);
    }
}

void Video::changeSize(int w, int h)
{
    Q_UNUSED(w); Q_UNUSED(h);
    QString smil_fit = SmilMedia->getFit().toLower();
    int fill_mode = NONE; // for do nothing
    if (smil_fit == "fill")
        fill_mode = Qt::IgnoreAspectRatio;
    else if (smil_fit == "meet")
        fill_mode = Qt::KeepAspectRatio;
    else if (smil_fit == "meetbest")
    {
//        if (loaded_image.width() >= w || loaded_image.height() > h)
//            fill_mode = Qt::KeepAspectRatio;
//       else
            fill_mode = NONE;
    }
    else if (smil_fit == "slice")
        fill_mode =  Qt::KeepAspectRatioByExpanding;

    media_item.data()->setProperty("fillMode",  fill_mode);
}
