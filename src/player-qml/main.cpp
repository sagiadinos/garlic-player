
#include "backend.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    qmlRegisterType<Backend>("com.sagiadinos.backend", 1, 0, "Backend");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    LibFacade    *MyLibFacade     = new LibFacade;
    QGuiApplication::setApplicationName(MyLibFacade->getConfiguration()->getAppName());
    QGuiApplication::setApplicationVersion(MyLibFacade->getConfiguration()->getVersion());
    QGuiApplication::setApplicationDisplayName(MyLibFacade->getConfiguration()->getAppName());
    MyLibFacade->getConfiguration()->determineIndexUri("http://indexes.smil-admin.com");

    QDir dir(".");
    MyLibFacade->getConfiguration()->determineBasePath(dir.absolutePath()); // Run in terminal could cause absolute path returns user homedirectory in QtCreator
    MyLibFacade->getConfiguration()->createDirectories();

    MyLibFacade->initIndex();
    MyLibFacade->checkForNewSmilIndex();
    Backend *MyBackend = new Backend;

    MyBackend->init(MyLibFacade, &engine);

    return app.exec();
}
