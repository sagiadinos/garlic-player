#include "network_dialog.h"

NetworkDialog::NetworkDialog(QWidget *parent, TConfiguration *Config) : QDialog(parent), ui(new Ui::NetworkDialog)
{
    ui->setupUi(this);
    MyConfiguration = Config;
    connect(ui->cb_connection, SIGNAL(currentIndexChanged(QString )), this, SLOT(changeIndex(QString )));
    connect(ui->chk_dhcp, SIGNAL(stateChanged(int)), this, SLOT(changeDHCP(int)));
    bool is_dhcp = hasActiveDHCP();
    ui->chk_dhcp->setChecked(is_dhcp);
    toggleIPSection(!is_dhcp);
}

void NetworkDialog::scanWiFiInterfaces()
{
    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface eth;
    foreach(eth, allInterfaces)
    {
        QProcess process1;
        QStringList arguments1;
#ifdef SUPPORT_RPI
    arguments1 << MyConfiguration->getPaths("scripts")+"/get_wifi_rpi.sh" << eth.name();
#else
    arguments1 << MyConfiguration->getPaths("scripts")+"/get_wifi.sh" << eth.name();
#endif
        process1.start("/bin/sh", arguments1);

        process1.waitForFinished(-1); // will wait forever until finished
        QString stdout = process1.readAllStandardOutput();
        QString stderr = process1.readAllStandardError();
        if (stderr == "" && stdout != "")
        {
            QString wifi_net;
            foreach(wifi_net, stdout.split("\n"))
            {
                if (wifi_net != "")
                    ui->cb_essid->addItem(eth.name() + ": " + wifi_net);
            }
        }
    }
}

void NetworkDialog::changeIndex(const QString &text)
{
    if (text == "LAN (cable)")
        toggleWifiSection(false);
    else
    {
        toggleWifiSection(true);
        scanWiFiInterfaces();
    }
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
    QDialog::accept();
}

bool NetworkDialog::hasActiveDHCP()
{
    // Unix check in Dir /run for a dhclient.pid
    QDir dir("/run");
    QStringList filter;
    filter << "dhclient*";
    QStringList found = dir.entryList(filter, QDir::Files);
    if (found.size() > 0)
        return true;
    else
        return false;
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
    ui->lb_domain->setVisible(is_visible);
    ui->lb_dns->setVisible(is_visible);
    ui->edit_ip->setVisible(is_visible);
    ui->edit_netmask->setVisible(is_visible);
    ui->edit_gateway->setVisible(is_visible);
    ui->edit_domain->setVisible(is_visible);
    ui->edit_dns->setVisible(is_visible);
}


