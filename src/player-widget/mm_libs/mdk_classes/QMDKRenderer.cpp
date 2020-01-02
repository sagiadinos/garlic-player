/*
 * Copyright (c) 2018-2019 WangBin <wbsecg1 at gmail.com>
 */
#include "QMDKRenderer.h"
#include "QMDKPlayer.h"
#include "mdk/Player.h"
#include <QCoreApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

using namespace mdk;
QMDKWindowRenderer::QMDKWindowRenderer(QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
{
}

QMDKWindowRenderer::~QMDKWindowRenderer() = default;

void QMDKWindowRenderer::setSource(QMDKPlayer* player)
{
    player->addRenderer(this);
    struct NoDeleter {
        void operator()(QMDKPlayer*) {}
    };
    player_.reset(player, NoDeleter());
    if (player) {
        // should skip rendering if player object is destroyed
        connect(player, &QObject::destroyed, [this](QObject* obj){
            auto p = static_cast<QMDKPlayer*>(obj); // why qobject_cast is null?
            if (player_.get() == p)
                player_.reset();
        });
    }
}

void QMDKWindowRenderer::initializeGL()
{
    // instance is destroyed before aboutToBeDestroyed(), and no current context in aboutToBeDestroyed()
    auto ctx = context();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, [ctx]{
        QOffscreenSurface s;
        s.create();
        ctx->makeCurrent(&s);
        Player::foreignGLContextDestroyed();
        ctx->doneCurrent();
    });
}

void QMDKWindowRenderer::resizeGL(int w, int h)
{
    auto p = player_;
    if (!p)
        return;
    p->addRenderer(this, w, h);
}

void QMDKWindowRenderer::paintGL()
{
    auto p = player_;
    if (!p)
        return;
    beforeGL();
    p->renderVideo(this);
    afterGL();
}

#ifdef QT_WIDGETS_LIB
QMDKWidgetRenderer::QMDKWidgetRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

QMDKWidgetRenderer::~QMDKWidgetRenderer() = default;

void QMDKWidgetRenderer::setSource(QMDKPlayer* player)
{
    player->addRenderer(this);
    player_ = player;
    if (player) {
        // should skip rendering if player object is destroyed
        connect(player, &QObject::destroyed, [this](QObject* obj){
            auto p = reinterpret_cast<QMDKPlayer*>(obj); // why qobject_cast is null? destroying and sub class dtor is finished?
            if (player_ == p)
                player_ = nullptr;
        });
    }
}

void QMDKWidgetRenderer::initializeGL()
{
    // instance is destroyed before aboutToBeDestroyed(), and no current context in aboutToBeDestroyed()
    auto ctx = context();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, [ctx]{
        QOffscreenSurface s;
        s.create();
        ctx->makeCurrent(&s);
        Player::foreignGLContextDestroyed();
        ctx->doneCurrent();
    });
}

void QMDKWidgetRenderer::resizeGL(int w, int h)
{
    if (!player_) // TODO: not safe. lock? but if player qobject is destroying, player dtor is finished. use true shared_ptr?
        return;
    player_->addRenderer(this, w, h);
}

void QMDKWidgetRenderer::paintGL()
{
    if (!player_) // safe, if player is destroyed, no update callback
        return;
    beforeGL();
    player_->renderVideo(this);
    afterGL();
}
#endif // QT_WIDGETS_LIB
