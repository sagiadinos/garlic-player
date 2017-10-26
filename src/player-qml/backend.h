#ifndef BACKEND_H
#define BACKEND_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickItem>

#include <QDebug>
#include "lib_facade.h"
#include "smilparser/media.h"

class Backend : public QObject
{
        Q_OBJECT
    public:
        explicit Backend(QObject *parent = nullptr);
        void init(LibFacade *lib_facade, QQmlApplicationEngine *engine);
    protected:
        LibFacade               *MyLibFacade;
        QQmlApplicationEngine   *MyEngine;
        QObject                 *root_item;
        QMap<QString, QVariant>  region_list_for_qml;

    public slots:
        void                      deleteRegionsAndLayouts();
        void                      setRegions(QList<Region> *region_list);
        void                      startShowMedia(TMedia *media);
        void                      stopShowMedia(TMedia *media);
};

#endif // BACKEND_H
