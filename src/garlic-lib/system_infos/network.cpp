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
#include "system_infos/network.h"

SystemInfos::Network::Network(QObject *parent) : QObject(parent)
{
    configurations_list = configuration_manager.allConfigurations(QNetworkConfiguration::Active);
    resetInterface();
}

bool SystemInfos::Network::resetInterface()
{
    if (countInterfaces() == 0)
        return false;
    configurations_list_iterator = configurations_list.begin();
    prepareCurrentInterface();
    return true;
}

bool SystemInfos::Network::nextInterface()
{
    configurations_list_iterator++;
    if (configurations_list_iterator == configurations_list.end())
        return false;
    prepareCurrentInterface();
    return true;
}

bool SystemInfos::Network::resetAddress()
{
    if (countAddresses() == 0)
        return false;
    addresses_list_iterator = addresses_list.begin();
    current_address = *addresses_list_iterator;
    return true;
}

bool SystemInfos::Network::nextAddress()
{
    addresses_list_iterator++;
    if (addresses_list_iterator == addresses_list.end())
        return false;
    current_address = *addresses_list_iterator;
    return true;
}

int SystemInfos::Network::countInterfaces()
{
   return configurations_list.size();
}

int SystemInfos::Network::countAddresses()
{
    return addresses_list.size();
}

bool SystemInfos::Network::isWifi()
{
    return (current_configuration.bearerType() == QNetworkConfiguration::BearerWLAN);
}

QString SystemInfos::Network::getInterfaceId()
{
    return current_interface.name();
}

QString SystemInfos::Network::getNetType()
{
    return current_configuration.bearerTypeName();
}

QString SystemInfos::Network::getMac()
{
    return current_interface.hardwareAddress();
}

QString SystemInfos::Network::getIP()
{
    return current_address.ip().toString();
}

QString SystemInfos::Network::getNetMask()
{
    return current_address.netmask().toString();
}

QString SystemInfos::Network::getBroadcast()
{
    return current_address.broadcast().toString();
}

void SystemInfos::Network::prepareCurrentInterface()
{
    current_configuration    = *configurations_list_iterator;
    QNetworkSession session(current_configuration);
    current_interface        = session.interface();
    addresses_list           = current_interface.addressEntries();
    resetAddress();
}
