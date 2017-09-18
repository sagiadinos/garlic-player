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
    setTimingAttributes();
    setBaseContainerAttributes();
    if (element.hasChildNodes())
    {
        active_element = element.firstChildElement();
        setPlaylist();
        if (active_element.tagName() == "metadata")
        {
            MyShuffle = new TShuffle(dom_list);
            MyShuffle->parse(active_element);
            shuffle = true;
        }

    }
    return true;
}

void TSeq::next(TBaseTiming *ended_element)
{
    childEnded(ended_element);
    if (!shuffle)
    {
        if (canGetNextItem())  // cause .end() pointing to the imaginary item after the last item in the list
        {
            active_element = getNextItem();
            reactByTag();
        }
        else // end check if repeat value is indefined or Limnit not reached
            handlePossibleRepeat();
    }
    else
    {
        if (MyShuffle->canGetNextItem())
        {
            active_element = MyShuffle->getNextItem();
            reactByTag();
        }
        else
            handlePossibleRepeat();
    }
}

bool TSeq::isChildPlayable(TBaseTiming *element)
{
    qDebug() << element->getID() << "is ChildPlayable in seq";
    childStarted(element);
    return true;
}

void TSeq::play()
{
    if (dom_list.size() > 0)
    {
        if (!shuffle)
        {
            iterator       = dom_list.begin();
            active_element = getNextItem();
        }
        else
        {
            active_element = MyShuffle->getNextItem();
        }
        reactByTag();
    }
    status = _playing;
}

void TSeq::resume()
{
    status = _playing;
}

void TSeq::stop()
{
    status = _stopped;
}

void TSeq::pause()
{
    status = _paused;
}

void TSeq::setDurationTimerBeforePlay()
{
    if (hasDurAttribute() || end_timer->isActive() || dom_list.size() > 0)
    {
        resetInternalRepeatCount();
        if (!is_resumed)
            emit startedContainer(parent_container, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
        setInternalDefaultDur();
}

bool TSeq::canGetNextItem()
{
    if (iterator != dom_list.end())
        return true;
    else
        return false;
}

QDomElement TSeq::getNextItem()
{
    if (iterator == dom_list.end())
        iterator = dom_list.begin();
    QDomElement ret = *iterator;
    iterator++;
    return ret;
}

void TSeq::handlePossibleRepeat()
{
    if (checkRepeatCountStatus())
    {
        qDebug() << getID() << "Repeats: " << count++;
        play();
    }
    else
        finishedActiveDuration();
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
    iterator = dom_list.begin();
}

