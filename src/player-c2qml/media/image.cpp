#include "media/image.h"

Image::Image(QQmlComponent *mc, QString r_id, QObject *parent) : PlayerBaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
    QString str(
                "import QtQuick 2.12\n \
                    Image {\n  \
                        id: "+getRegionId()+"_image; \
                        anchors.fill: parent; \n \
                        signal qmlSignal(string msg); \n \
                        onStatusChanged: qmlSignal("+getRegionId()+"_image.status); \n \
                    }\n"
    );
    image_item.reset(createMediaItem(mc, str));

    connect(image_item.data(), SIGNAL(qmlSignal(QString)), this, SLOT(doStateChanged(QString)));
    // set anchors.fill via Javascript do not work in Android cause parent and this are NULL
    // unfortunately anchors.fill cannot be accessed from C++ so it has to initialized static via
    // property var img_fill_mode: ""; \n
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
        image_item.data()->setProperty("fillMode", determineFillMode(SmilMedia->getFit()));
        if (SmilMedia->getLogContentId() != "")
            setStartTime();
    }
}

void Image::deinit()
{
    if (SmilMedia->getLogContentId() != "")
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
        return PRESERVEASPECTCROP;
    else if (smil_fit == "meetbest")
        return PRESERVEASPECTFIT;
    else
        return STRETCH;
}

void Image::doStateChanged(QString state)
{
    qDebug() << state << "doStateChanged";
}
