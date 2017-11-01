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
#ifndef NETWORK_ACCESS_H
#define NETWORK_ACCESS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include "tools/logging_categories.h"

class TNetworkAccess : public QObject
{
        Q_OBJECT
    public:
        explicit TNetworkAccess(QByteArray agent, QObject *parent = nullptr);
        // getter/setter
        QByteArray getUserAgent();
        void       setUserAgent(QByteArray value);
        QUrl       getRemoteFileUrl();
        void       setRemoteFileUrl(QUrl value);

    protected:
        QByteArray        user_agent;
        QUrl              remote_file_url;
        QNetworkRequest   prepareNetworkRequest(QUrl remote_url);

    signals:
        void              succeed(TNetworkAccess *);
        void              failed(TNetworkAccess *);
};

#endif // NETWORK_ACCESS_H
