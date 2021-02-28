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

#include "base_media.h"

BaseMedia::BaseMedia(Files::MediaManager *mm, MainConfiguration *config, QObject *parent) : BaseTimings(parent)
{
    MyMainConfiguration = config;
    MyMediaManager      = mm;
}

void BaseMedia::preloadParse(QDomElement element)
{
    root_element   = element;
    setAttributes();     // special for every media type
    parseBaseParameters(); // in this class
}


QString BaseMedia::getLoadablePath()
{
    // local media should be played always regardless of the cache mode
    if (MyMediaManager->checkCacheStatus(src) == MEDIA_IS_LOCAL)
    {
        return MyMediaManager->requestLoadablePath(src);
    }

    if (getCacheControl() == CACHE_CONTROL_ONLY_IF_CACHED)
    {
        if (MyMediaManager->checkCacheStatus(src) == MEDIA_CACHED)
        {
            return MyMediaManager->requestLoadablePath(src);
        }
        else
        {
            return ""; // only cachable media can be loaded
        }
    }
    else
    {
        if (MyMediaManager->checkCacheStatus(src) == MEDIA_UNCACHABLE)
        {
            return src;
        }
        else
        {
            return MyMediaManager->requestLoadablePath(src);
        }

    }
}

void BaseMedia::play()
{
    status = _playing;
}

void BaseMedia::registerInMediaManager()
{
    MyMediaManager->registerFile(src);
}

void BaseMedia::registerInMediaManagerAsUncachable()
{
    MyMediaManager->registerAsUncachable(src);
}


void BaseMedia::resume()
{
    play();
}

QString BaseMedia::getParamsAsQuery() const
{
    QString ret;
    if (params_as_query.length() > 0)
        ret = "?" + params_as_query.join("&");
    return ret;
}

void BaseMedia::pause()
{
    status = _paused;
}

void BaseMedia::stop()
{
    status = _stopped;
}

void BaseMedia::parseBaseMediaAttributes()
{
    parseTimingAttributes();

    region = getAttributeFromRootElement("region");
    fit    = getAttributeFromRootElement("fit");
    type   = getAttributeFromRootElement("type");
    MyExpr.setExpression(getAttributeFromRootElement("expr"));
    src    = getAttributeFromRootElement("src");
}

void BaseMedia::emitfinished()
{
  emit finishedMedia(parent_container, this);
}

void BaseMedia::parseBaseParameters()
{
    if (!root_element.hasChildNodes())
     return;

    QDomNodeList childs = root_element.childNodes();
    for (int i = 0; i < childs.length(); i++)
    {
        setAdditionalParameters(childs.item(i).toElement());
    }
}

void BaseMedia::finishedDuration()
{
    if (!isEndTimerActive())
    {
        finishedActiveDuration();
    }
}

void BaseMedia::setAdditionalParameters(QDomElement param)
{
    if (param.tagName() != "param" || !param.hasAttribute("name") || !param.hasAttribute("value"))
        return;

    if (param.attribute("name") == "cacheControl")
    {
        cache_control = determineCacheControl(param.attribute("value"));
    }
    else if (param.attribute("name") == "filename")
    {
        filename = param.attribute("value");
    }
    else if (param.attribute("name") == "logContentId")
    {
        log_content_id = param.attribute("value");
    }
}

int BaseMedia::determineCacheControl(QString value)
{
    if (value.toUpper() == "ONLYIFCACHED")
    {
        return CACHE_CONTROL_ONLY_IF_CACHED;
    }
    return CACHE_CONTROL_USE_CACHE;
}

