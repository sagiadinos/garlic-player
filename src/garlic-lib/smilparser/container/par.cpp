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

#include "par.h"

TPar::TPar(TContainer *pc, QObject *parent) : TContainer(parent)
{
    parent_container = pc;
    setObjectName("TPar");
}

TPar::~TPar()
{
}

void TPar::preloadParse(QDomElement element)
{
    root_element   = element;
    parseTimingAttributes();
    if (element.hasChildNodes())
    {
        traverseChilds();
    }
}

void TPar::prepareDurationTimerBeforePlay()
{
    if (startDurTimer() || isEndTimerActive() || childs_list.size() > 0)
    {
        resetInternalRepeatCount();
        emit startedContainer(parent_container, this);
    }
    else
    {
        skipElement();
    }
}

bool TPar::isChildPlayable(BaseTimings *element)
{
    childStarted(element);
    return true;
}



/**
 * @brief TPar::next means that it looks if there are no active elements
 *        when nothing is active (anymore) and no repeat possible
 *        then the finished signal will emitted;
 * @return
 */
void TPar::next(BaseTimings *ended_element)
{
    childEnded(ended_element);
    if (!hasPlayingChilds())
    {
        handlePossibleRepeat();
    }
    return;
}

BaseTimings *TPar::getChildElementFromList()
{
    QSet<BaseTimings *>::iterator i = activatable_childs.begin();
    return *i;
}

void TPar::play()
{
    for (childs_list_iterator =  childs_list.begin(); childs_list_iterator < childs_list.end(); childs_list_iterator++)
    {
        active_element = *childs_list_iterator;
        emitFoundElement();
    }
    status = _playing;
}

void TPar::resume()
{
    status = _playing;
}

void TPar::pause()
{
    status = _paused;
}

void TPar::stop()
{
    status = _stopped;
}


void TPar::traverseChilds()
{
    // put all playlist elements into a QList
    QDomNodeList childs = root_element.childNodes();
    count_childs        = childs.length();
    for (int i = 0; i < count_childs; i++)
    {
        if (childs.item(i).toElement().tagName() != "") // e.g. comments
        {
            childs_list.append(childs.item(i).toElement());
            emit preloadElement(this, childs.item(i).toElement());
        }
    }
}

void TPar::handlePossibleRepeat()
{
    // when a Dur time is active ignore repeat
    // https://www.w3.org/TR/SMIL3/smil-timing.html#q75
    if (isDurTimerActive())
        return;

    if(checkRepeatCountStatus())
    {
        play();
    }
    else
    {
        finishedActiveDuration();
    }
}

void TPar::finishedDuration()
{
    if (!isEndTimerActive())
    {
        finishedActiveDuration();
    }
}

