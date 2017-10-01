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

#ifndef TBASE_H
#define TBASE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtXml>
#include "tools/logging_categories.h"

/**
 * @brief The abstract TBase class should inherited for all smil elements in body section
 *        Here comes is the init of all common attributes:
 *        id/xml:id, title, class and xml:lang
 *
 */
class TBase : public QObject
{
    Q_OBJECT
public:
    explicit TBase(QObject *parent = 0);
    virtual bool          parse(QDomElement element) = 0;
            QString       getID(){return id;}
            QString       getTitle(){return title;}
            QString       getLang(){return lang;}
            QString       getClass(){return a_class;}
            QDomElement   getRootElement(){return root_element;}
    static  QString       parseID(QDomElement element);
protected:
            QString       id             = "";
            QString       title          = "";
            QString       lang           = "";
            QString       a_class        = ""; // cause class is reserved word. a_class means attribut_class
            void          setBaseAttributes();
            QDomElement   root_element;
};

#endif // TBASE_H
