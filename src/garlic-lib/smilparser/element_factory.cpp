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
#include "element_factory.h"

ElementFactory::ElementFactory(Files::MediaManager *mm, MainConfiguration *config, QObject *parent) : QObject(parent)
{
    MyMediaManager      = mm;
    MyMainConfiguration = config;
}

BaseTimings* ElementFactory::createBase(QDomElement dom_element, TContainer *parent_container, QObject *parent)
{
    QString tag_name = dom_element.tagName();
    QString type     = "";
    if(tag_name == "img" || tag_name == "video" || tag_name == "audio" || tag_name == "text" || tag_name == "prefetch" ||
            tag_name == "seq" || tag_name == "par" || tag_name == "body" || tag_name == "excl" )
    {
        type = tag_name;
    }
    else if(tag_name == "ref" && dom_element.hasAttribute("type"))
    {
        if(dom_element.attribute("type").contains("image"))
           type = "img";
        else if (dom_element.attribute("type").contains("video"))
           type = "video";
        else if (dom_element.attribute("type").contains("audio"))
           type = "audio";
        else if (dom_element.attribute("type").contains("text"))
           type = "text";
        else if (dom_element.attribute("type").contains("application/widget"))
           type = "widget";
    }
    else if(tag_name == "ref" && dom_element.hasAttribute("src") && dom_element.attribute("src").contains("adapi:"))
    {
        type = "ref_command";
    }

    if (type == "img")
    {
        return new TImage(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "video")
    {
        return new TVideo(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "audio")
    {
        return new TAudio(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "text")
    {
        return new TWeb(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "widget")
    {
        return new TWidget(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "prefetch")
    {
        return new TPrefetch(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "ref_command")
    {
        return new TRefCommand(parent_container, MyMediaManager, MyMainConfiguration, parent);
    }
    else if (type == "seq")
    {
        return new TSeq(parent_container, parent);
    }
    else if (type == "par")
    {
        return new TPar(parent_container, parent);
    }
    else if (type == "excl")
    {
        return new TExcl(parent_container, parent);
    }
    else if (type == "body")
    {
        return new TBody(parent);
    }

    return new Unknown(parent_container, MyMediaManager, MyMainConfiguration, parent);
}
