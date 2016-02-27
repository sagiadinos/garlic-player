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

#include "seq.h"

TSeq::TSeq(TContainer * parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TSeq");
}

bool TSeq::parse(QDomElement element)
{
    root_element   = element;
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        active_element = element.firstChildElement();
        if (active_element.tagName() == "metadata")
            doMetaData();
        setPlaylist();
        iterator       = dom_list.begin();
        active_element = *iterator;
    }
    return true;
}

bool TSeq::isChildPlayable(TBase *element)
{
    qDebug() << element->getID() <<QTime::currentTime().toString() << "is ChildPlayable in seq";
    childStarted(element);
    return true;
}


void TSeq::setDurationTimerBeforePlay()
{
    if (hasDurAttribute() || end_timer->isActive() || dom_list.size() > 0)
    {
        resetInternalRepeatCount();
        if (!is_resumed)
            emit startedPlaylist(parent_container, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
        finishedActiveDuration();
    return;
}

void TSeq::play()
{
    iterator       = dom_list.begin();
    active_element = *iterator;
    reactByTag();
    status = _playing;
    return;
}

void TSeq::resume()
{
    status = _playing;
    return;
}


void TSeq::stop()
{
    status = _stopped;
    return;
}

void TSeq::pause()
{
    status = _paused;
    return;
}

void TSeq::doMetaData()
{
    QDomNodeList childs = root_element.childNodes();
    QDomElement  element;
    int          length = childs.length();
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() == "meta" && element.hasAttribute("name") && element.hasAttribute("content"))
        {
            if (element.attribute("name") == "adapi:pickingAlgorithm")
                pickingAlgorithm = element.attribute("content");
            else if (element.attribute("name") == "adapi:pickingBehavior")
                pickingBehavior = element.attribute("content");
            else if (element.attribute("name") == "adapi:pickNumber")
                pickNumber = element.attribute("content").toInt();
        }
    }
    return;
}

void TSeq::next(TBase *ended_element)
{
    childEnded(ended_element);
    iterator++; // inc iterator first only when inc result smaller than  .end()
    if (iterator < dom_list.end())  // cause .end() pointing to the imaginary item after the last item in the vector
    {
        active_element = *iterator;
        reactByTag();
    }
    else // end check if repeat value is indefined or Limnit not reached
    {
        if (checkRepeatCountStatus())
        {
            play();
        }
        else
            finishedActiveDuration();
    }
    return;
}

bool TSeq::previous()
{
    return false;
}

void TSeq::setPlaylist()
{
    QDomNodeList childs = root_element.childNodes();
    int          length = childs.length();
    QDomElement  element;
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "metadata" && element.tagName() != "") // e.g. comments
            dom_list.append(element);
    }
    if (pickingAlgorithm == "shuffle" && pickingBehavior == "pickN" && pickNumber > 0)
    {
        randomizePlaylist();
        random = true;
        internal_pick = pickNumber;
    }
    return;
}

void TSeq::randomizePlaylist()
{
    // Knuth shuffle
    int length = dom_list.length();
    for (int i = length - 1; i > 0; --i)
    {
        int i2 = qrand() % (i + 1);
        std::swap(dom_list[i], dom_list[i2]);
    }
    // Let only pickNumber items in List
    for (int i = 0; i < (length-pickNumber); i++)
        dom_list.removeLast();
    return;
}
