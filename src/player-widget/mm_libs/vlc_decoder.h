#ifdef SUPPORT_LIBVLC

#ifndef VLC_DECODER_H
#define VLC_DECODER_H

#include <QObject>
#include <QObject>
#include <QTimer>
#include <vlc/vlc.h>
#include "mediawidget_wrapper.h"
#include "tools/logging_categories.h"

class VlcDecoder: public QObject
{
    Q_OBJECT
public:
    VlcDecoder(QObject *parent = nullptr);
    void                setVideoOutput(MediaWidgetWrapper *renderer);
    void                removeVideoOutput(MediaWidgetWrapper *renderer);
    bool                load(QString file_path);
    void                setVolume(QString percent);
    void                unload();

signals:
    void                finished();
public slots:
    void                play();
    void                stop();

protected:
    QString             current_media_path;
    libvlc_instance_t       *vlcInstance;
    libvlc_media_player_t   *vlcPlayer;
    MediaWidgetWrapper       *Renderer;

protected slots:
    void updatePosition();
    void displayErrorMessage();
};
#endif

#endif // VLC_DECODER_H
