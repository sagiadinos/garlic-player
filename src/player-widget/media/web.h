#ifndef WEB_H
#define WEB_H

#include <QWebEngineView>
#include <QWebEngineSettings>

#include "smilparser/web.h"
#include "base_media.h"

class Web : public BaseMedia
{
        Q_OBJECT
    public:
        Web(QObject *parent = nullptr);
        ~Web();
        void            init(TMedia *media);
        void            deinit();
        void            changeSize(int w, int h);
        QWidget        *getView();
    protected:
        QWebEngineView  *browser;
    protected slots:
        void             doLoadFinished(bool has_succeeded);
};

#endif // WEB_H
