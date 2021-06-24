#include "media/image.h"

Image::Image(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);
    QString str(
                "import QtQuick 2.12\n \
                    Image {  \
                        cache: false; \
                        id: "+getRegionId()+"_image; \
                        anchors.fill: parent; \n \
                }\n"
    );
    image_item.reset(createMediaItem(mc, str));
 }

Image::~Image()
{
    image_item.reset();
}

void Image::init(BaseMedia *media)
{
    SmilMedia = media;
    if (load(image_item.data()))
    {
        image_item.data()->setProperty("fillMode", determineFillMode(SmilMedia->getFit().toLower()));
        if (SmilMedia->getLogContentId() != "")
            setStartTime();
    }
}

void Image::deinit()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();
    image_item.data()->setProperty("source","");
}

void Image::setParentItem(QQuickItem *parent)
{
    image_item.data()->setParentItem(parent);
}

// Image.Stretch - the image is scaled to fit
// Image.PreserveAspectFit - the image is scaled uniformly to fit without cropping
// Image.PreserveAspectCrop - the image is scaled uniformly to fill, cropping if necessary
int Image::determineFillMode(QString smil_fit)
{
    if (smil_fit == "fill")
        return STRETCH;
    else if (smil_fit == "meet")
        return PRESERVEASPECTFIT;
    else if (smil_fit == "meetbest")
        return PRESERVEASPECTFIT;
    else if (smil_fit == "slice")
        return PRESERVEASPECTCROP;
    else
        return NONE;
}
