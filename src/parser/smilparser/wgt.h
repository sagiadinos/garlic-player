#ifndef WGT_H
#define WGT_H


class TWgt : public TMedia
{
    Q_OBJECT
public:
    TWgt();
    void     pause();
    void     stop();
    void     play();
    QWebEngineView *getBrowser(){return browser;}
public slots:
    void     setDurationTimerBeforePlay();
protected:
    QWebEngineView *browser;
    bool            loadMedia();
    void            setAttributes();
};

#endif // WGT_H
