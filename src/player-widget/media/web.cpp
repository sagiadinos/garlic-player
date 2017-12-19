/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
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
#include "web.h"

Web::Web(QObject *parent) : BaseMedia(parent)
{
}

Web::~Web()
{
    // FIX THIS! delete or deleteLater for browser crashes,
    // when playliste changed and browser was used method init/deinit
    // it crashes even as stack variable or QScopedPointer
    // actual solution works, but unsatisfied

    // Info: delete crashes cause this destructor invoked two times
    // deleteLater crashes someone else deep in assembler


    qDebug() << "delete browser";
}

void Web::init(TMedia *media)
{
    MyMedia = media;
    browser = new QWebEngineView;
    connect(browser, SIGNAL(loadFinished(bool)), this, SLOT(doLoadFinished(bool)));

    QUrl url(media->getLoadablePath());
    browser->load(url);
    if (MyMedia->getLogContentId() != "")
        setStartTime();
}

void Web::deinit()
{
    browser->load(QUrl(""));
    browser->close();
    delete  browser;
    if (MyMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void Web::changeSize(int w, int h)
{
    browser->resize(w-2, h-2);
}

QWidget *Web::getView()
{
    return browser;
}

void Web::doLoadFinished(bool has_succeeded)
{
    if (has_succeeded)
    {
        QString code = QStringLiteral("document.body.style.overflow = 'hidden';");
        browser->page()->runJavaScript(code);
    }
}
