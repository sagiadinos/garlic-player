#ifndef PLAYERCONFIGURATION_H
#define PLAYERCONFIGURATION_H


#include <QApplication>
#include <QObject>
#include "tools/main_configuration.h"

class PlayerConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit PlayerConfiguration(MainConfiguration *mc, QObject *parent = nullptr);

    void determineInitConfigValues();
    bool hasLauncher(){ return has_launcher;}
    void setHasLauncher(bool value);
    void setUuidFromLauncher(QString value);
    void setSmilIndexUriFromLauncher(QString value);


private:

    bool       has_launcher            = false;;
    QString    launcher_uuid           = "";
    QString    launcher_smil_index_uri = "";
    MainConfiguration *MyMainConfiguration;
    void       determineUuid();
    void       determineSmilIndexUri();
    void       determinePlayerName();

signals:

};

#endif // PLAYERCONFIGURATION_H
