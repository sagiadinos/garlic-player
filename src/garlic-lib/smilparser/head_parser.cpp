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

#include "head_parser.h"

HeadParser::HeadParser(MainConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration = config;
    setDefaultValues();
}

HeadParser::~HeadParser()
{
    setDefaultValues();
}

void HeadParser::setDefaultValues()
{
    refresh                        = 0; // means no looking for new index
    title                          = "No Title";
    width                          = 1366;
    height                         = 768;
    backgroundColor                = "black";
    region_list.clear();
    default_region.regionName      = "screen";
    default_region.top             = 0;
    default_region.left            = 0;
    default_region.width           = 1;
    default_region.height          = 1;
    default_region.z_index         = 0;
    default_region.backgroundColor = "transparent";
    region_list.append(default_region);            void process();

}

QString HeadParser::getTitle()
{
    return title;
}

QString HeadParser::getRootBackgroundColor()
{
    return backgroundColor;
}

QList<Region> *HeadParser::getLayout()
{
    return &region_list;
}

void HeadParser::parse(QDomElement head, SmilHead::TaskScheduler *MyTasks)
{
    setDefaultValues();
    if (head.hasChildNodes())
    {
        QDomNodeList childs = head.childNodes();
        QDomElement element;
        for (int i = 0; i < childs.length(); i++)
        {
            element = childs.item(i).toElement();
            if (element.tagName() == "meta")
                parseMeta(element);
            else if (element.tagName() == "metadata")
                parseMetaData(element, MyTasks);
            else if (element.tagName() == "layout")
                parseLayout(element);
        }
    }
}

void HeadParser::setRootLayout(int w, int h)
{
    width           = w;
    height          = h;
    return;
}

void HeadParser::setInventoryTable(DB::InventoryTable *value)
{
    MyInventoryTable = value;
}

void HeadParser::parseMeta(QDomElement element)
{
    if (element.hasAttribute("name") && element.attribute("name") == "title" && element.hasAttribute("content"))
        title = element.attribute("content");
    else if (element.hasAttribute("http-equiv") && element.attribute("http-equiv") == "Refresh" && element.hasAttribute("content"))
    {
        refresh = element.attribute("content").toInt();
        #ifdef QT_NO_DEBUG
            if (refresh < 31) // make sure that impaciently clients do not stress the cms too much but only in release mode
                refresh = 30;
        #endif
    }
}

void HeadParser::parseMetaData(QDomElement element, SmilHead::TaskScheduler *MyTasks)
{
    QDomNodeList node_list = element.elementsByTagName("subscription");

    // QObject's have not a copy constructor so subscriptions had to be stored in a subscription_list
    // otherwise we get only the last parsed subscription send and mem leaks
    SubScription *subscription  = new SubScription(this);

    // FixMe: Violation SRP Find a way to replace this with a GoF-Pattern, e.g. factory!
    // Remind Task scheduler
    for(int i = 0; i < node_list.size(); i++)
    {
        subscription->parse(node_list.at(i).toElement());
        if (subscription->getType() == "SystemReport")
        {
            MySystemReportManager.reset(new Reporting::SystemReportManager(MyConfiguration));
            MySystemReportManager.data()->init(subscription->getAction(), subscription->getRefreshInterval());
        }
        else if (subscription->getType() == "InventoryReport" && MyInventoryTable != Q_NULLPTR)
        {
            MyInventoryReportManager.reset(new Reporting::InventoryReportManager(MyConfiguration, MyInventoryTable));
            MyInventoryReportManager.data()->init(subscription->getAction(), subscription->getRefreshInterval());
        }
        else if (subscription->getType() == "PlaylogCollection")
        {
            MyPlayLogsManager.reset(new Reporting::PlayLogsManager(MyConfiguration));
            MyPlayLogsManager.data()->init(subscription->getAction(), subscription->getRefreshInterval());
        }
        else if (subscription->getType() == "EventlogCollection")
        {
            MyEventLogsManager.reset(new Reporting::EventLogsManager(MyConfiguration));
            MyEventLogsManager.data()->init(subscription->getAction(), subscription->getRefreshInterval());
        }
        else if (subscription->getType() == "TaskSchedule")
        {
            MyTasks->processFromUrl(QUrl(subscription->getAction()));
        }
    }
}

void HeadParser::parseLayout(QDomElement layout)
{
    // root-layout must be found first, cause regions based on it
    QDomNodeList nodelist = layout.elementsByTagName("root-layout");
    if (nodelist.length() == 1)
       parseRootLayout(nodelist.item(0).toElement());
    nodelist = layout.elementsByTagName("region");
    if (nodelist.length() > 0)
        parseRegions(nodelist);
}

void HeadParser::parseRootLayout(QDomElement root_layout)
{
    if (root_layout.hasAttribute("width"))
        width  = root_layout.attribute("width").toInt();
    if (root_layout.hasAttribute("height"))
        height = root_layout.attribute("height").toInt();
    if (root_layout.hasAttribute("backgroundColor"))
        backgroundColor = root_layout.attribute("backgroundColor");
}

void HeadParser::parseRegions(QDomNodeList childs)
{
    QDomElement element;
    region_list.clear(); // clear from default values
    for (int i = 0; i < childs.length(); i++)
    {
        Region region = default_region; // init with default values
        element = childs.item(i).toElement();
        if (element.tagName() != "region")
            continue;
        if (element.hasAttribute("regionName"))
        {
            region.regionName      = element.attribute("regionName");
            if (element.hasAttribute("top"))
                region.top             = calculatePercentBasedOnRoot(element.attribute("top"), height);
            if (element.hasAttribute("left"))
                region.left            = calculatePercentBasedOnRoot(element.attribute("left"), width);
            if (element.hasAttribute("width"))
                region.width           = calculatePercentBasedOnRoot(element.attribute("width"), width);
            if (element.hasAttribute("height"))
                region.height          = calculatePercentBasedOnRoot(element.attribute("height"), height);
            if (element.hasAttribute("z-index"))
                region.z_index         = element.attribute("z-index").toInt();
            if (element.hasAttribute("backgroundColor"))
                region.backgroundColor = element.attribute("backgroundColor");
            region_list.append(region);
            std::sort(region_list.begin(), region_list.end()); // sort z-indexes ascending to place Widgets corect in mainwindow
        }
        else // when one region-tag has no regionName break the loop delete QList and put the default_region 100% region as multizone
        {
            region_list.clear();
            region_list.append(default_region);
            break;
        }

    }
    qDebug() << childs.length() << "regions found in SMIL layout-tag and " << region_list.length() << " regions were initialized ";
}



qreal HeadParser::calculatePercentBasedOnRoot(QString value, qreal root)
{
    int length  = value.length();
    int px      = 0;
    if (value[length-1] == QChar('%'))
        return qreal(value.mid(0, length-1).toFloat())/100; // to have a easy calculatable value for resize zones
    else if (value[length-1] == QChar('x'))
        px = value.mid(0, length-2).toInt();
    else
        px = value.toInt();
    qreal ret;
    if (root > 0)
      ret = (100*px) / root;
    else
      ret = 0;
    return ret/100; // to have a easy calculatable value for resize zones
}

