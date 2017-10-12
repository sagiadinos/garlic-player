#include "web.h"

Web::Web(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyWeb  = qobject_cast<TWeb *>(media);

}

Web::~Web()
{
    delete browser;
}

void Web::init()
{
    browser      = new QWebEngineView;
    QUrl url(MyWeb->getLoadablePath());
    browser->load(url);
    browser->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
}

void Web::changeSize(int w, int h)
{
    browser->resize(w-2, h-2);
}

QWidget *Web::getView()
{
    return browser;
}
