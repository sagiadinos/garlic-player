#ifndef WEB_H
#define WEB_H

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineProfile>

#include "smilparser/media/widget.h"
#include "smilparser/media/web.h"
#include "player_base_media.h"

class PlayerWeb : public PlayerBaseMedia
{
    Q_OBJECT
    public:
        PlayerWeb(QObject *parent = nullptr);
        ~PlayerWeb();
        void            init(BaseMedia *media, Region *reg);
        void            deinit();
        void            changeSize(int w, int h);
        QWidget        *getView();
    protected:
        QWebEngineView *browser;
    protected slots:
        void             doLoadFinished(bool has_succeeded);
    private:
        QString          sanitizeUri(QString uri);
};

#endif // WEB_H
