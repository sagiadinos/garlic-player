#ifndef WEB_H
#define WEB_H

#include <QObject>
#include <QWebEngineView>
#include <QWebEngineSettings>

#include "smilparser/web.h"
#include "base_media.h"

class Web : public BaseMedia
{
        Q_OBJECT
    public:
        Web(TMedia *media, QObject *parent = nullptr);
        ~Web();
        void            init();
        void            changeSize(int w, int h);
        QWidget        *getView();
    protected:
        TWeb             *MyWeb;
        QWebEngineView   *browser;
};

#endif // WEB_H
