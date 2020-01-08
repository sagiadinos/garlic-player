#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include "configuration.h"
#include "xml_configuration.h"

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

    class TaskScheduler : public BaseManager
    {
            Q_OBJECT
        public:
            explicit TaskScheduler(TConfiguration *config, QObject *parent = nullptr);
            void init(QString action);

    protected:
            TConfiguration *MyConfiguration;
            Downloader     *TaskFileDownloader;
            QUrl            task_scheduler_url;
            QDomDocument    document;
            QScopedPointer<SmilHead::XMLConfiguration> MyXMLConfiguration;
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
            void doSucceedParse(TNetworkAccess *network);
            void emitApplyConfiguration();
            void emitUpdateFirware();
            void emitShutdownPlayer();

    signals:
        void                   applyConfiguration();
        void                   updateFirmware();
        void                   shutdownPlayer();
    };
}
#endif // TASKSCHEDULER_H
