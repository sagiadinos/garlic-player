#ifndef NETWORK_DIALOG_H
#define NETWORK_DIALOG_H

#include <QObject>
#include <QDialog>
#include <QDir>
#include <QNetworkInterface>
#include <QProcess>

#include <QDebug>

#include "tools/configuration.h"

namespace Ui
{
    class NetworkDialog;
}

#include "ui_networkdialog.h"

class NetworkDialog : public QDialog
{
        Q_OBJECT
    public:
        explicit NetworkDialog(QWidget *parent, TConfiguration *Config);

    protected:
        Ui::NetworkDialog *ui;
        TConfiguration    *MyConfiguration = NULL;
        void scanWiFiInterfaces();
        bool commitToSystem();
    protected slots:
        void changeIndex(const QString &text);
        void changeDHCP(int state);
        void accept();
    private:
        bool hasActiveDHCP();
        void toggleWifiSection(bool is_visible);
        void toggleIPSection(bool is_visible);
        void writeWPASupplicantConf();
        void writeDHCPConf();
        QString determineInterface();
        QString determineESSID();

    signals:

};

#endif // NETWORK_DIALOG_H
