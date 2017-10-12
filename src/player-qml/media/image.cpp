#include "media/image.h"

Image::Image(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    ParserImage  = qobject_cast<TImage *>(media);
}

Image::~Image()
{
    delete image_item;
}

void Image::init(QQmlComponent *mc)
{
    QString fill_mode = determineQmlFillMode(ParserImage->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";
    QString str(
                "import QtQuick 2.7\n \
                    Image {\n " +
                            anchors+
                        "fillMode: Image."+fill_mode+";\n \
                        source: \"file:"+ParserImage->getLoadablePath()+"\";\n \
                    }\n"
    );
    image_item = createMediaItem(mc, str);
}

void Image::setParentItem(QQuickItem *parent)
{
    image_item->setParentItem(parent);
}
