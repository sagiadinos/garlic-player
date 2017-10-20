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
#include "general.h"

SystemInfos::General::General(QObject *parent) : QObject(parent)
{

}

int SystemInfos::General::countThreads()
{
    int ret = 0;;
#if defined Q_OS_WIN32

#elif defined Q_OS_DARWIN

#elif defined Q_OS_LINUX // work on Android too
    QFile fp("/proc/self/stat");
    if (!fp.open(QIODevice::ReadOnly))
        return 0;		/* Can't open? */
    QTextStream in(&fp);
    QStringList fields = in.readLine().split(" ");
    fp.close();
    ret = fields.at(19).toInt();
#endif
    return ret;
}
