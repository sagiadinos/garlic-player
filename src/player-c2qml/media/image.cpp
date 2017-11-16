#include "media/image.h"

Image::Image(QQmlComponent *mc, QString r_id, QObject *parent) : BaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
    QString str(
                "import QtQuick 2.7\n \
                    Image {\n  \
                        id: "+getRegionId()+"_image;\n \
                        source: \"file:\"; \n \
                        anchors.fill: parent; \n \
                    }\n"
    );
    image_item.reset(createMediaItem(mc, str));
    // do not work in Android cause parent and this are NULL
    // unfortunately anchors.fill cannot be accessed from C++ so it has to initialized
    // property var img_fill_mode: ""; \n
    // onStatusChanged:
    // {
    //    parent.parent.fitImage(this, video_fill_mode);
    // }
}

Image::~Image()
{
    image_item.reset();
}

void Image::init(TMedia *media)
{
    MyImage = qobject_cast<TImage *>(media);
    QString source = "file:"+MyImage->getLoadablePath();
    image_item.data()->setProperty("source", source);
    image_item.data()->setProperty("fillMode", determineFillMode(MyImage->getFit()));
}

void Image::deinit()
{
    image_item.data()->setProperty("source", QUrl());
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
