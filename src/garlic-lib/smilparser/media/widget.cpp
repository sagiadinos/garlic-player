/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2021 Nikolaos Sagiadinos <ns@smil-control.com>
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
#include "widget.h"

TWidget::TWidget(Files::MediaManager *mm, MainConfiguration *config, SmilHead::PlaceHolder *ph, QObject *parent) : BaseMedia(mm, config, ph, parent)
{
    setObjectName("TWidget");
    is_media = true;
}


TWidget::~TWidget()
{
}

void TWidget::prepareDurationTimers()
{
    if (!MyExpr.executeQuery() || getLoadablePath().isEmpty())
    {
        skipElement();
        return;
    }

    if (status == _active && !isRestartable())
        return;

    if (startDurTimer() || hasEndTimer())
    {
        resetInternalRepeatCount();
        emitStartElementSignal(this);
    }
    else
    {
        skipElement();
    }
}

// ====================  protected methods =================================

void TWidget::setAttributes()
{
    parseBaseMediaAttributes();
    src  = getAttributeFromRootElement("src", "");
    params_as_query.clear();
    QDomNodeList childs = root_element.childNodes();
    for (int i = 0; i < childs.length(); i++)
    {
        parseWidgetCallParameters(childs.item(i).toElement());
    }
}

void TWidget::parseWidgetCallParameters(QDomElement param)
{
    if (param.tagName() != "param" || !param.hasAttribute("name") || !param.hasAttribute("value"))
        return;

    QString attribute_name = param.attribute("name");
    if (attribute_name == "cacheControl" || attribute_name == "filename" || attribute_name == "logContentId")
        return;

    params_as_query.append(attribute_name + "=" + QUrl::toPercentEncoding(param.attribute("value")));
}

void TWidget::prepareDurationTimersForRepeat()
{
    startDurTimer();
}

// ====================  private methods =================================
