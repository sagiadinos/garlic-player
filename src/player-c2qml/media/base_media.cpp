#include "base_media.h"

BaseMedia::BaseMedia(QObject *parent) : QObject(parent)
{
}

QString BaseMedia::determineQmlFillMode(QString fill_mode)
{
    if (fill_mode == "fill")
       return "Stretch";
    else if (fill_mode == "meet")
        return "PreserveAspectCrop";
    else if (fill_mode == "meetbest")
        return "PreserveAspectFit";
    else
        return "Pad";
}

QQuickItem *BaseMedia::createMediaItem(QQmlComponent *mc, QString str)
{
    mc->setData(str.toUtf8(), QUrl());
    return qobject_cast<QQuickItem *>(mc->create());
}
