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

#include "media.h"

TMedia::TMedia(TBaseTiming * parent)
{
    Q_UNUSED(parent);
}

bool TMedia::parse(QDomElement element)
{
    root_element   = element;
    setAttributes();     // special for every media type
    setBaseParameters(); // in this class
    loaded       = false;
    return true;
}

QString TMedia::parseSrc(QDomElement element)
{
    QString ret = "";
    if (element.hasAttribute("src"))
        ret = element.attribute("src");
    return ret;
}

bool TMedia::load()
{
    int stat = MyFileManager->checkCacheStatus(src);
    if (!loaded)
    {
        if(stat > 0)
            loaded = loadMedia();
    }
    else
    {
        if(stat == MyFileManager->_reloadable)
            loaded = loadMedia();
    }

    return loaded;
}


void TMedia::registerFile(TFileManager *FileManager)
{
    MyFileManager = FileManager;
    MyFileManager->registerFile(src);
    return;
}

void TMedia::setBaseMediaAttributes()
{
    region = title = src = exec = "";
    if (!root_element.hasAttribute("begin") && parent_container->objectName() == "TExcl")
        begin.parse("indefinite");
    if (root_element.hasAttribute("region"))
        region = root_element.attribute("region");
    if (root_element.hasAttribute("fit"))
        fit = root_element.attribute("fit");
    src = parseSrc(root_element);
    if (root_element.hasAttribute("exec"))
        exec = root_element.attribute("exec");
    setTimingAttributes();
    return;
}

void TMedia::emitfinished() // called from finishedActiveDuration() in TBase
{
   emit finishedMedia(parent_container, this);
   return;
}

void TMedia::setBaseParameters()
{
    cache_control = filename = log_content_id = "";
    if (root_element.hasChildNodes())
    {
        QDomNodeList childs = root_element.childNodes();
        QDomElement param;
        for (int i = 0; i < childs.length(); i++)
        {
            param = childs.item(i).toElement();
            if (param.tagName() == "param")
            {
                if (param.hasAttribute("cacheControl") && param.hasAttribute("value"))
                    cache_control = param.attribute("value");
                else if (param.hasAttribute("filename") && param.hasAttribute("value"))
                    filename = param.attribute("value");
                else if (param.hasAttribute("logContentId") && param.hasAttribute("value"))
                    log_content_id = param.attribute("value");
            }
        }

    }
}
