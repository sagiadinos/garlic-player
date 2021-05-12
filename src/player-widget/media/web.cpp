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

PlayerWeb::PlayerWeb(QObject *parent) : PlayerBaseMedia(parent)
{
}

PlayerWeb::~PlayerWeb()
{
    // FIX THIS! delete or deleteLater for browser crashes,
    // when playliste changed and browser was used method init/deinit
    // it crashes even as stack variable or QScopedPointer
    // actual solution works, but unsatisfied

    // Info: delete crashes cause this destructor invoked two times
    // deleteLater crashes someone else deep in assembler


    qDebug() << "delete browser";
}

void PlayerWeb::init(BaseMedia *media)
{
    SmilMedia = media;
    browser   = new QWebEngineView;
    connect(browser, SIGNAL(loadFinished(bool)), this, SLOT(doLoadFinished(bool)));

    // Deactivate caching for testing
    // browser->page()->profile()->setHttpCacheType(QWebEngineProfile::NoCache);
    browser->page()->setBackgroundColor(Qt::transparent);

    browser->settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false); // auto play video and audio
    browser->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    browser->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true); // needed to block SOP Attention can be deprecated look at param functions in main.cpp

// activates everything for debug and testing
/*
    browser->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    browser->settings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::TouchIconsEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, true);
    browser->settings()->setAttribute(QWebEngineSettings::PrintElementBackgrounds, true);
    browser->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    browser->settings()->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);
    browser->settings()->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, true);
    browser->settings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, true);
    browser->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
*/

    QString uri = sanitizeUri(media->getLoadablePath()) + media->getParamsAsQuery();
    browser->load(uri);
    qDebug() << "The Uri: " + uri;
    if (SmilMedia->getLogContentId() != "")
        setStartTime();
}

void PlayerWeb::deinit()
{
    if (browser == Q_NULLPTR)
        return;

    browser->load(QUrl(""));
    browser->close();
    delete  browser;
    browser = Q_NULLPTR;
    if (SmilMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void PlayerWeb::changeSize(int w, int h)
{
    browser->resize(w-2, h-2);
}

QWidget *PlayerWeb::getView()
{
    return browser;
}

void PlayerWeb::doLoadFinished(bool has_succeeded)
{
    if (has_succeeded)
    {
        QString code = QStringLiteral("document.body.style.overflow = 'hidden';");
        browser->page()->runJavaScript(code);
    }
}

QString PlayerWeb::sanitizeUri(QString uri)
{
    if (uri.isEmpty())
        return "";
    // add file scheme if neccessary
    if (uri.at(0) == "/")
    {
        uri = "file://" + uri;
    }
    return uri;
}
