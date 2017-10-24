#include "web.h"

Web::Web(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    // should stay a raw pointer cause it belongs to Parser
    MyWeb = qobject_cast<TWeb *>(media);
}

Web::~Web()
{

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
    web_item.reset(createMediaItem(mc, str));
}

void Web::setParentItem(QQuickItem *parent)
{
    web_item.data()->setParentItem(parent);
}
