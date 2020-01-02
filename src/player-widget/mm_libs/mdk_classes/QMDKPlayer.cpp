/*
 * Copyright (c) 2018-2019 WangBin <wbsecg1 at gmail.com>
 * MDK SDK with QOpenGLWindow example
 */
#include "QMDKPlayer.h"
#include <QCoreApplication>
#include <QStringList>
#include <QtDebug>
#include <QRegion>
#include "mdk/Player.h"

using namespace MDK_NS;
QMDKPlayer::QMDKPlayer(QObject *parent)
    : QObject(parent)
    , player_(new Player())
{
    setLogHandler([](LogLevel level, const char* msg){
        if (level >= std::underlying_type<LogLevel>::type(LogLevel::Info)) {
            qDebug() << msg;
        } else if (level >= std::underlying_type<LogLevel>::type(LogLevel::Warning)) {
            qWarning() << msg;
        }
    });
    player_->setRenderCallback([](void* vo_opaque){
        auto vo = reinterpret_cast<QObject*>(vo_opaque);
        if (!vo->isWidgetType()) { // isWidgetType() is fastest, and no need to include <QWidget>
            if (vo->isWindowType())
                QCoreApplication::instance()->postEvent(vo, new QEvent(QEvent::UpdateRequest));
            return;
        }
        class QUpdateLaterEvent final : public QEvent {   
        public:
            explicit QUpdateLaterEvent(const QRegion& paintRegion)
                : QEvent(UpdateLater), m_region(paintRegion)
            {} 
            ~QUpdateLaterEvent() {}
            inline const QRegion &region() const { return m_region; }
        protected:
            QRegion m_region;
        };
        QCoreApplication::instance()->postEvent(vo, new QUpdateLaterEvent(QRegion(0, 0, vo->property("width").toInt(), vo->property("height").toInt())));
    });
}

QMDKPlayer::~QMDKPlayer() = default;

void QMDKPlayer::setDecoders(const QStringList &dec)
{
    std::vector<std::string> v;
    foreach (QString d, dec) {
        v.push_back(d.toStdString());
    }
    player_->setVideoDecoders(v);
}

void QMDKPlayer::setMedia(const QString &url)
{
    player_->setMedia(url.toUtf8().constData());
}

void QMDKPlayer::setVolume(float value)
{
    player_->setVolume(value);
}

void QMDKPlayer::play()
{
    player_->setState(State::Playing);
}

void QMDKPlayer::pause()
{
    player_->setState(State::Paused);
}

void QMDKPlayer::stop()
{
    player_->setState(State::Stopped);
}

bool QMDKPlayer::isPaused() const
{
    return player_->state() == State::Paused;
}

void QMDKPlayer::seek(qint64 ms)
{
    player_->seek(ms);
}

qint64 QMDKPlayer::position() const
{
    return player_->position();
}

void QMDKPlayer::addRenderer(QObject* vo, int w, int h)
{
    if (w <= 0)
        w = vo->property("width").toInt();
    if (h <= 0)
        h = vo->property("height").toInt();
    player_->setVideoSurfaceSize(w, h, vo); // call update cb
}

void QMDKPlayer::renderVideo(QObject* vo)
{
    player_->renderVideo(vo);
}
