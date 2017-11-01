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
#include "index_model.h"

IndexModel::IndexModel(QObject *parent) : QObject(parent)
{
}

QDomElement IndexModel::getHead()
{
    return getTag("head");
}

QDomElement IndexModel::getBody()
{
    return getTag("body");
}

bool IndexModel::loadDocument(QString file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical(ContentManager) << "SMIL_OPEN_ERROR resourceUri:" << file_path << "Fail to load as SMIL index";
        return false;
    }
    if (!document.setContent(&file))
    {
        qCritical(ContentManager) << "SMIL_OPEN_ERROR resourceUri:" << file_path << "SMIL index syntax error or other";
        file.close();
        return false;
    }
    file.close();
    return true;
}

QDomDocument IndexModel::getDocument() const
{
    return document;
}

void IndexModel::setDocument(const QDomDocument &value)
{
    document = value;
}

// ==================  protected methods =======================================

QDomElement IndexModel::getTag(QString tag_name)
{
    QDomNodeList nodelist = document.elementsByTagName(tag_name);
    if (nodelist.length() == 0) // fallback for ensure giving an empty named Tag-element back
        return document.createElement(tag_name);

    return nodelist.item(0).toElement();
}