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
        Web(QObject *parent = nullptr);
        ~Web();
        void            init(TMedia *media);
        void            deinit();
        void            changeSize(int w, int h);
        QWidget        *getView();
    protected:
        TWeb            *ParserWeb;
        QWebEngineView  *browser; // needed when opointer deleteLater not to crash
};

#endif // WEB_H
