#include "media/image.h"

Image::Image(QQmlComponent *mc, QString r_id, QObject *parent) : BaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
    QString str(
                "import QtQuick 2.7\n \
                    Image {\n  \
                        id: "+getRegionId()+"_image;\n \
                        property var img_fill_mode: ""; \n \
                        source: \"file:\"; \n \
                        onStatusChanged: \n \
                        {    \n \
                            if (this.status == Image.Ready) \
                                parent.parent.fitImage(this, img_fill_mode) \n \
                        } \n \
                    }\n"
    );
    image_item.reset(createMediaItem(mc, str));
}

Image::~Image()
{
    image_item.reset();
}

void Image::init(TMedia *media)
{
    MyImage = qobject_cast<TImage *>(media);
    QString source = "file:"+MyImage->getLoadablePath();
    image_item.data()->setProperty("img_fill_mode", MyImage->getFit());
    image_item.data()->setProperty("source", source);
}

void Image::deinit()
{
    image_item.data()->setProperty("source", QUrl());
}

void Image::setParentItem(QQuickItem *parent)
{
    image_item.data()->setParentItem(parent);
}
