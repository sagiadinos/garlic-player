#include "web.h"

Web::Web(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyWeb  = qobject_cast<TWeb *>(media);

}

Web::~Web()
{
    delete web_item;
}

void Web::init(QQmlComponent *mc)
{
    QString str("import QtQuick 2.7\n \
                 import QtWebView 1.1\n \
                    WebView {\n \
                        anchors.fill: parent;\n \
                        url: \""+MyWeb->getLoadablePath()+"\";\n \
                   }\n"
    );
    web_item = createMediaItem(mc, str);
}

void Web::setParentItem(QQuickItem *parent)
{
    web_item->setParentItem(parent);
}
