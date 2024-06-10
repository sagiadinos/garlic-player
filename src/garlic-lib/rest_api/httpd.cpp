/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2024 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/
#include "httpd.h"
#include "httplistener.h"
#include "request_mapper.h"

using namespace stefanfrings;

RestApi::Httpd::Httpd(LibFacade *lf, QObject *parent) : QObject(parent)
{
    MyLibFacade      = lf;
    initDefaultSettings();
}

void RestApi::Httpd::init(QCoreApplication *app)
{
    determineHttpdSettings(MyLibFacade->getConfiguration()->getUserConfig());
    new HttpListener(&HttpdSettings, new RequestMapper(MyLibFacade, &HttpdSettings, app), app);
}

void RestApi::Httpd::determineHttpdSettings(QSettings *settings)
{
    settings->beginGroup("httpd");
    QStringList keys = settings->allKeys();
    for (const QString& key : keys)
    {
        HttpdSettings.setValue(key, settings->value(key));
    }
    settings->endGroup();
}

void RestApi::Httpd::initDefaultSettings()
{
  // for later
  //  if (MyLibFacade->getConfiguration()->getUserConfig()->childGroups().contains("httpd"))
  //      return;

   // MyConfiguration->setUserConfigByKey("host","127.0.0.1");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/port","8080");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/minThreads","1");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/maxThreads","4");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/cleanupInterval","60000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/readTimeout","60000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/maxRequestSize","16000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/maxMultiPartSize","10000000");

    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/path", MyLibFacade->getConfiguration()->getPaths("cache"));
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/encoding", "UTF-8");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/maxAge", "9000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/cacheTime", "60000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/cacheSize", "1000000");
    MyLibFacade->getConfiguration()->setUserConfigByKey("httpd/maxCachedFileSize", "65536");

}
