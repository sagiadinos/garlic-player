#ifdef SUPPORT_MDK
#ifndef MDK_DECODER_H
#define MDK_DECODER_H

#include <QObject>
#include <QTimer>
#include "mediawidget_wrapper.h"
#include "tools/logging_categories.h"
#include <memory>
#include "mdk_classes/QMDKPlayer.h"

class MdkDecoder : public QObject
{
    Q_OBJECT
public:
    explicit MdkDecoder(QObject *parent = nullptr);
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
    QString              current_media_path;
    MediaWidgetWrapper  *Renderer;
    QScopedPointer<QMDKPlayer, QScopedPointerDeleteLater>   MediaDecoder;

protected slots:
    void updatePosition();
    void displayErrorMessage();
};

#endif // MDK_DECODER_H

#endif
