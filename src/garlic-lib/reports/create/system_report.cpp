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
#include "system_report.h"

Reporting::CreateSystemReport::CreateSystemReport(TConfiguration *config,
                           QObject *parent) : Reporting::CreateBase(config, parent)
{
    MyConfiguration = config;
    MyDiscSpace.reset(new DiscSpace(MyConfiguration->getPaths("cache")));
    MyMemory.reset(new SystemInfos::Memory(this));
    MyNetwork.reset(new SystemInfos::Network(this));

}

void Reporting::CreateSystemReport::process()
{
    init();
    createSystemInfo();
    createNetwork();
    createConfiguration();
    createModelInfo();
    createFactoryDefault();
    createUserPref();
    createHardwareInfo();
}

QString Reporting::CreateSystemReport::asXMLString()
{
    return document.toString();
}


void Reporting::CreateSystemReport::createSystemInfo()
{
    system_info = document.createElement("systemInfo");
    player.appendChild(system_info);

    system_info.appendChild(createTagWithTextValue("systemStartTime", MyConfiguration->getStartTime()));
    system_info.appendChild(createTagWithTextValue("systemTimeZone", MyConfiguration->getTimeZone()));

    MyDiscSpace->init(MyConfiguration->getPaths("cache"));
    system_info.appendChild(createTagWithTextValue("totalCapacity", QString::number(MyDiscSpace->getStorageBytesTotal())));
    system_info.appendChild(createTagWithTextValue("totalFreeSpace", QString::number(MyDiscSpace->getStorageBytesAvailable())));

    system_info.appendChild(createTagWithTextValue("cpuUsage", ""));

    MyMemory->refresh();
    system_info.appendChild(createTagWithTextValue("memoryTotal", QString::number(MyMemory->getTotal())));
    system_info.appendChild(createTagWithTextValue("memoryUsed", QString::number(MyMemory->getUsed())));
    system_info.appendChild(createTagWithTextValue("hdmiOutput", ""));

}

void Reporting::CreateSystemReport::createNetwork()
{
    network = document.createElement("network");
    system_info.appendChild(network);
    MyNetwork->resetInterface();
    for(int i = 0; i < MyNetwork->countInterfaces(); i++)
    {
        appendNetworkChilds();
        MyNetwork->nextInterface();
    }
}

void Reporting::CreateSystemReport::createConfiguration()
{
    configuration = document.createElement("configuration");
    system_info.appendChild(configuration);
}

void Reporting::CreateSystemReport::createModelInfo()
{
    QDomElement model_info = document.createElement("modelInfo");
    configuration.appendChild(model_info);
    model_info.appendChild(createPropTag("PCB", ""));
    model_info.appendChild(createPropTag("PCBRevision", ""));
    model_info.appendChild(createPropTag("operatingSystem", MyConfiguration->getOS()));
    model_info.appendChild(createPropTag("manufacturer", ""));
    model_info.appendChild(createPropTag("manufacturerURL", ""));
    model_info.appendChild(createPropTag("modelDescription", ""));
    model_info.appendChild(createPropTag("modelName", ""));
    model_info.appendChild(createPropTag("modelURL", ""));
    model_info.appendChild(createPropTag("option", ""));
}

void Reporting::CreateSystemReport::createFactoryDefault()
{
    QDomElement factory_default = document.createElement("factoryDefault");
    configuration.appendChild(factory_default);
    factory_default.appendChild(createTagWithTextValue("variant", ""));
}

void Reporting::CreateSystemReport::createUserPref()
{
    QDomElement user_pref = document.createElement("userPref");
    configuration.appendChild(user_pref);
    user_pref.appendChild(createPropTag("info.playerName", MyConfiguration->getPlayerName()));
    user_pref.appendChild(createPropTag("info.playGroup", ""));
    user_pref.appendChild(createPropTag("info.playGroupMaster", ""));
    user_pref.appendChild(createPropTag("system.locale", QLocale::system().name()));

    user_pref.appendChild(createPropTag("content.serverUrl", MyConfiguration->getIndexUri()));
}

void Reporting::CreateSystemReport::createHardwareInfo()
{
    hardware_info = document.createElement("hardwareInfo");
    player.appendChild(hardware_info);
    QDomElement hardware = document.createElement("hardware");
    hardware_info.appendChild(hardware);

    // TODO integrate TScreen Class into lib
    hardware.setAttribute("id", "display:0");
    hardware.appendChild(createPropTag("modelName", ""));
    hardware.appendChild(createPropTag("product_id", ""));
    hardware.appendChild(createPropTag("serialNumber", ""));
    hardware.appendChild(createPropTag("vendorId", ""));
}


void Reporting::CreateSystemReport::appendNetworkChilds()
{
    interface = document.createElement("interface");
    interface.setAttribute("id", MyNetwork->getInterfaceId());
    network.appendChild(interface);
    interface.appendChild(createTagWithTextValue("type", MyNetwork->getNetType()));
    interface.appendChild(createTagWithTextValue("mac", MyNetwork->getMac()));
    for(int i = 0; i < MyNetwork->countAddresses(); i++)
    {
        appendNetworkAddressChilds();
        MyNetwork->nextAddress();
    }
}

void Reporting::CreateSystemReport::appendNetworkAddressChilds()
{
    interface.appendChild(createTagWithTextValue("ip", MyNetwork->getIP()));
    interface.appendChild(createTagWithTextValue("netmask", MyNetwork->getNetMask()));
    interface.appendChild(createTagWithTextValue("broadcast", MyNetwork->getBroadcast()));
}
