#include "httpd.h"

Httpd::Httpd(MainConfiguration *mc, QObject *parent) : QObject(parent)
{
    MyConfiguration = mc;
}

void Httpd::init(QCoreApplication *app)
{
    // MyConfiguration->setUserConfigByKey("host","192.168.0.100");
    MyConfiguration->setUserConfigByKey("port","8080");
    MyConfiguration->setUserConfigByKey("minThreads","1");
    MyConfiguration->setUserConfigByKey("maxThreads","4");
    MyConfiguration->setUserConfigByKey("cleanupInterval","60000");
    MyConfiguration->setUserConfigByKey("readTimeout","60000");
    MyConfiguration->setUserConfigByKey("maxRequestSize","16000");
    MyConfiguration->setUserConfigByKey("maxMultiPartSize","10000000");

    new HttpListener(MyConfiguration->getUserConfig(), new RequestMapper(MyConfiguration, app), app);

}
