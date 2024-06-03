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
