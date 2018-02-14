#include "network_dialog.h"

NetworkDialog::NetworkDialog(QWidget *parent, TConfiguration *Config) : QDialog(parent), ui(new Ui::NetworkDialog)
{
    ui->setupUi(this);
    MyConfiguration = Config;
    connect(ui->cb_interface, SIGNAL(currentIndexChanged(QString )), this, SLOT(changeIndex(QString )));
    connect(ui->chk_dhcp, SIGNAL(stateChanged(int)), this, SLOT(changeDHCP(int)));

    bool is_dhcp = hasActiveDHCP();

    scanInterfaces();

    ui->chk_dhcp->setChecked(is_dhcp);
    toggleIPSection(!is_dhcp);
}

bool NetworkDialog::scanPossibleWiFiInterfaces(const QString interface)
{
    QProcess    process;
    QStringList arguments;
    bool        is_wifi =false;

    ui->cb_essid->clear();

#ifdef SUPPORT_RPI
    arguments << MyConfiguration->getPaths("scripts")+"/get_wifi_rpi.sh" << interface;
#else
    arguments << MyConfiguration->getPaths("scripts")+"/get_wifi.sh" << interface;
#endif
    process.start("/bin/sh", arguments);

    process.waitForFinished(-1); // will wait forever until finished
    QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();
    if (stderr == "" && stdout != "")
    {
        QString wifi_net;
        foreach(wifi_net, stdout.split("\n"))
        {
            if (wifi_net != "")
                ui->cb_essid->addItem(wifi_net);
        }
        ui->cb_essid->setCurrentText(determineCurrentESSID());
        is_wifi = true;
    }
    return is_wifi;
}

bool NetworkDialog::commitToSystem()
{
    MyConfiguration->setNetworkInterface(ui->cb_interface->currentText());
    if (ui->cb_essid->isVisible()) // Is WiFi
    {
        if (ui->cb_essid->currentText() == "")
        {
            ui->lb_error->setText(tr("ESSID missed"));
            return false;
        }
        if (ui->edit_passphrase->text().length() < 8)
        {
            ui->lb_error->setText(tr("Passphrase must have at least 8 Letters"));
            return false;
        }

        writeWPASupplicantConf();
    }
    if (ui->chk_dhcp->checkState() == Qt::Unchecked)
    {
        QPair<QHostAddress, int> addr = QHostAddress::parseSubnet(ui->edit_ip->text()+"/"+ui->edit_netmask->text());
        if (addr.first.isNull())
        {
            ui->lb_error->setText(tr("IP is incorrect"));
            return false;
        }
        if (addr.second == -1)
        {
            ui->lb_error->setText(tr("Netmask is incorrect"));
            return false;
        }
        QHostAddress gateway(ui->edit_gateway->text());
        if (gateway.isNull())
        {
            ui->lb_error->setText(tr("Gateway is incorrect"));
            return false;
        }
        writeStaticIntoDHCPConf();
    }
    return true;
}

void NetworkDialog::changeIndex(const QString &text)
{
    toggleWifiSection(scanPossibleWiFiInterfaces(text));
}

void NetworkDialog::changeDHCP(int state)
{
    if (state == Qt::Checked)
        toggleIPSection(false);
    else
        toggleIPSection(true);
}

void NetworkDialog::accept()
{
    ui->lb_error->setText("");
    if (commitToSystem())
        QDialog::accept();
}

bool NetworkDialog::hasActiveDHCP()
{
    QProcess process;
    QStringList arguments;

    QPair<QHostAddress, int> addr = QHostAddress::parseSubnet(ui->edit_ip->text()+"/"+ui->edit_netmask->text());
#ifdef SUPPORT_RPI
    arguments << MyConfiguration->getPaths("scripts")+"/is_dhcp_rpi.sh";
#else
    arguments << MyConfiguration->getPaths("scripts")+"/is_dhcp.sh";
#endif
    process.start("/bin/sh", arguments);
    process.waitForFinished(-1); // will wait forever until finished
    return (process.exitCode() == 1);
}

void NetworkDialog::toggleWifiSection(bool is_visible)
{
    ui->lb_essid->setVisible(is_visible);
    ui->cb_essid->setVisible(is_visible);
    ui->lb_encryption->setVisible(is_visible);
    ui->cb_encryption->setVisible(is_visible);
    ui->lb_passphrase->setVisible(is_visible);
    ui->edit_passphrase->setVisible(is_visible);
}

void NetworkDialog::toggleIPSection(bool is_visible)
{
    ui->lb_ip->setVisible(is_visible);
    ui->lb_netmask->setVisible(is_visible);
    ui->lb_gateway->setVisible(is_visible);
    ui->lb_dns->setVisible(is_visible);
    ui->edit_ip->setVisible(is_visible);
    ui->edit_netmask->setVisible(is_visible);
    ui->edit_gateway->setVisible(is_visible);
    ui->edit_dns->setVisible(is_visible);
}

void NetworkDialog::writeWPASupplicantConf()
{
    QProcess process;
    QStringList arguments;

    arguments << MyConfiguration->getPaths("scripts")+"/write_wpa.sh" << ui->cb_essid->currentText() << ui->edit_passphrase->text();
    process.start("/bin/sh", arguments);

    process.waitForFinished(-1); // will wait forever until finished
}

void NetworkDialog::writeStaticIntoDHCPConf()
{
    QProcess process;
    QStringList arguments;

    QPair<QHostAddress, int> addr = QHostAddress::parseSubnet(ui->edit_ip->text()+"/"+ui->edit_netmask->text());
    arguments << MyConfiguration->getPaths("scripts")+"/write_dhcp.sh"
              << addr.first.toString()
              << QString::number(addr.second)
              << ui->edit_gateway->text()
              << ui->edit_dns->text()
    ;
    process.start("/bin/sh", arguments);

    process.waitForFinished(-1); // will wait forever until finished

}

void NetworkDialog::determinceStaticFromDHCPConf()
{
    QFile dhcp_conf("/etc/dhcpcd.conf");
    if (!dhcp_conf.open(QIODevice::ReadOnly))
        return;
    QString file_contents(dhcp_conf.readAll());
    QStringList garlic_contents = file_contents.mid(file_contents.indexOf("#garlic dhcp-static-config")).split("\n");
    if (garlic_contents.size() == 0)
        return;

    QString line;
    foreach(line, garlic_contents)
    {
        if(line.contains("static ip_address="))
        {
            QStringList ip = seperateValueFromParameter(line).split("/");
            ui->edit_ip->setText(ip.at(0));
            ui->edit_gateway->setText(determineIPFromSuffix(ip.at(1).toInt()));
        }
        if(line.contains("static routers="))
            ui->edit_gateway->setText(seperateValueFromParameter(line));
        if(line.contains("static domain_name_servers="))
            ui->edit_dns->setText(seperateValueFromParameter(line));
    }

}

void NetworkDialog::scanInterfaces()
{
    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface eth;
    QString current_interface = MyConfiguration->getNetworkInterface();
    foreach(eth, allInterfaces)
    {
        if (eth.flags().testFlag(QNetworkInterface::IsUp)
            && eth.flags().testFlag(QNetworkInterface::CanBroadcast)
            && eth.flags().testFlag(QNetworkInterface::CanMulticast)
        )
            ui->cb_interface->addItem(eth.name());

    }
    ui->cb_interface->setCurrentText(current_interface);
}

QString NetworkDialog::seperateValueFromParameter(const QString line)
{
    QStringList list = line.split("=");
    return list.at(1);
}

QString NetworkDialog::determineCurrentESSID()
{
    QProcess process;
    QStringList arguments;

    arguments << ui->cb_interface->currentText() << "--raw";
    process.start("iwgetid", arguments);

    process.waitForFinished(-1); // will wait forever until finished
    QString ret = process.readAllStandardOutput();
    return ret.trimmed();
}

QString NetworkDialog::determineIPFromSuffix(int suffix)
{
    switch (suffix)
    {
        case 32: return "255.255.255.255";
        case 31: return "255.255.255.254";
        case 30: return "255.255.255.252";
        case 29: return "255.255.255.248";
        case 28: return "255.255.255.240";
        case 27: return "255.255.255.224";
        case 26: return "255.255.255.192";
        case 25: return "255.255.255.128";
        case 24: return "255.255.255.0";
        case 23: return "255.255.254.0";
        case 22: return "255.255.252.0";
        case 21: return "255.255.248.0";
        case 20: return "255.255.240.0";
        case 19: return "255.255.224.0";
        case 18: return "255.255.192.0";
        case 17: return "255.255.128.0";
        case 16: return "255.255.0.0";
        case 15: return "255.254.0.0";
        case 14: return "255.252.0.0";
        case 13: return "255.248.0.0";
        case 12: return "255.240.0.0";
        case 11: return "255.224.0.0";
        case 10: return "255.192.0.0";
        case 9: return "255.128.0.0";
        case 8: return "255.0.0.0";
        case 7: return "254.0.0.0";
        case 6: return "252.0.0.0";
        case 5: return "248.0.0.0";
        case 4: return "240.0.0.0";
        case 3: return "224.0.0.0";
        case 2: return "192.0.0.0";
        case 1: return "128.0.0.0";
        default: return "255.255.255.0";
    }


}
