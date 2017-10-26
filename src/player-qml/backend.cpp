#include "backend.h"
/**
 * @brief Backend::Backend
 *      No dependency injection in constructor cause qmlRegisterType (if sometimes neccessary)
 *      can only handle contructors with QObject
 * @param parent
 */

Backend::Backend(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Backend::init
 *      For dependency injections
 * @param lib_facade
 * @param engine
 */
void Backend::init(LibFacade *lib_facade, QQmlApplicationEngine *engine)
{
    MyLibFacade = lib_facade;
    MyEngine    = engine;
    connect(MyLibFacade, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(newIndexLoaded()), this, SLOT(deleteRegionsAndLayouts()));
    connect(MyLibFacade, SIGNAL(newIndexPrepared(QList<Region> *)), this, SLOT(setRegions(QList<Region> *)));
    root_item = MyEngine->rootObjects().at(0);

    // Todo: Look if it should be called from QML
    MyLibFacade->initIndex();
    MyLibFacade->checkForNewSmilIndex();
}

void Backend::deleteRegionsAndLayouts()
{
 //   qDeleteAll(region_list_for_qml);
    region_list_for_qml.clear();
    QVariant returned_value;
    QMetaObject::invokeMethod(root_item, "deleteRegionsAndLayouts", Q_RETURN_ARG(QVariant, returned_value));
    MyLibFacade->prepareNewLoadedIndex(); // should called from QML
}

void Backend::setRegions(QList<Region> *region_list)
{
    for (QList<Region>::iterator i = region_list->begin(); i != region_list->end(); i++)
    {
        Region r = *i;
        QStringList ui;
        ui << r.regionName << QString::number(r.top) << QString::number(r.left) << QString::number(r.width) << QString::number(r.height) << QString::number(r.z_index) << r.backgroundColor;
        region_list_for_qml.insert(r.regionName, ui);
    }
    QVariant returned_value;
    QMetaObject::invokeMethod(root_item, "setRegions", Q_RETURN_ARG(QVariant, returned_value),  Q_ARG(QVariant, region_list_for_qml));
    MyLibFacade->beginSmilBodyParsing(); // parse not before Layout ist build to prevent crash in MainWindow::startShowMedia
}

void Backend::startShowMedia(TMedia *media)
{
}

void Backend::startShowMedia(TMedia *media)
{
    if (region_list_for_qml.size() == 0 )
        return;

    QMap<QString, QVariant> MyMedia;
    MyMedia.insert("id", media->getID());
    MyMedia.insert("type", media->objectName());
    MyMedia.insert("region_name", media->getRegion());
    MyMedia.insert("loadable_path", media->getLoadablePath());
    MyMedia.insert("fill_mode", media->getFit());

    QVariant returned_value;
    QMetaObject::invokeMethod(root_item, "startShowMedia", Q_RETURN_ARG(QVariant, returned_value),  Q_ARG(QVariant, MyMedia));
}

void Backend::stopShowMedia(TMedia *media)
{
    if (region_list_for_qml.size() == 0 )
        return;

    QMap<QString, QVariant> MyMedia;

    MyMedia.insert("id", media->getID());
    MyMedia.insert("type", media->objectName());
    MyMedia.insert("region_name", media->getRegion());
    MyMedia.insert("loadable_path", media->getLoadablePath());
    MyMedia.insert("fill_mode", media->getFit());

    QVariant returned_value;
    QMetaObject::invokeMethod(root_item, "stopShowMedia", Q_RETURN_ARG(QVariant, returned_value),  Q_ARG(QVariant, MyMedia));
}

