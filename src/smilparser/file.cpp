/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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

#include "file.h"

TFile::TFile()
{

}

QString TFile::loadToDom(QString filename)
{
    QFile file(filename);
    QString ret = "";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "Fail to open file";
    else
    {
        if (!document.setContent(&file))
            qDebug() << "Fail load to a DomDocument";
        file.close();
        QFileInfo fi(filename);
        ret = fi.absolutePath()+"/";

    }
    return ret;
}

QDomElement TFile::getHead()
{
    return getTag("head");
}

QDomElement TFile::getBody()
{
    return getTag("body");
}

QDomElement TFile::getTag(QString tag_name)
{
    QDomNodeList nodelist = document.elementsByTagName(tag_name);
    QDomElement tag;
    if (nodelist.length() == 1)
       tag = nodelist.item(0).toElement();
    return tag;
}
