#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include "main_configuration.h"
#include "xml_configuration.h"
#include "firmware_downloader.h"

namespace SmilHead
{
    struct UpdateSettings
    {
        QString task_id          = "";
        QUrl    source_uri       = QUrl();
        QString content_type     = "";
        int     content_length   = 0;
        QString content_checksum = "";
        QString checksum_method  = "MD5";
    };
    struct FirmwareUpdate
    {
        QString task_id          = "";
        QUrl    source_uri       = QUrl();
        QString version          = "";
        QString content_type     = "";
        int     content_length   = 0;
        QString content_checksum = "";
        QString checksum_method  = "MD5";
    };
    struct applyCommand
    {
        QString task_id          = "";
        QString command          = "";
    };
    struct ShutdownPlayer
    {
        QString task_id          = "";
        QString content_type     = "reboot";
    };

    class TaskScheduler : public Files::BaseManager
    {
            Q_OBJECT
        public:
            explicit TaskScheduler(MainConfiguration *config, QObject *parent = nullptr);
            void processFromUrl(QUrl url);

    protected:
            MainConfiguration *MyConfiguration;
            Downloader     *TaskFileDownloader;
            QDomDocument    document;
            QScopedPointer<SmilHead::XMLConfiguration> MyXMLConfiguration;
            QScopedPointer<SmilHead::FirmwareDownloader> MyFirmwareDownloader;
            UpdateSettings  MyUpdateSetting;
            FirmwareUpdate  MyFirmwareUpdate;
            ShutdownPlayer  MyShutdownPlayer;
            void            parseFirmwareUpdate(QDomElement element);
            void            parseUpdateSetting(QDomElement element);
            void            parseShutdownPlayer(QDomElement element);
          // for later  void            parseApplyCommand(QDomElement update_settings);
            bool            hasUsedTaskId(QString task_id, QString task_name);
            bool            loadDocument(QString file_path);
    protected slots:
            void doSucceed(TNetworkAccess *network);
            void emitApplyConfiguration();
            void emitInstallSoftware(QString file_path);
            void emitShutdownPlayer();

    signals:
        void                   applyConfiguration();
        void                   installSoftware(QString file_path);
        void                   reboot();
    };
}
#endif // TASKSCHEDULER_H
