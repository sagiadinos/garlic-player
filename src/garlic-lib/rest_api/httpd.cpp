#include "httpd.h"

RestApi::Httpd::Httpd(LibFacade *lf, QObject *parent) : QObject(parent)
{
    MyLibFacade      = lf;
}

void RestApi::Httpd::init(QCoreApplication *app)
{
    // MyConfiguration->setUserConfigByKey("host","192.168.0.100");
    MyLibFacade->getConfiguration()->setUserConfigByKey("port","8080");
    MyLibFacade->getConfiguration()->setUserConfigByKey("minThreads","1");
    MyLibFacade->getConfiguration()->setUserConfigByKey("maxThreads","4");
    MyLibFacade->getConfiguration()->setUserConfigByKey("cleanupInterval","60000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("readTimeout","60000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("maxRequestSize","16000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("maxMultiPartSize","10000000");

    new HttpListener(MyLibFacade->getConfiguration()->getUserConfig(), new RequestMapper(MyLibFacade, app), app);

}
