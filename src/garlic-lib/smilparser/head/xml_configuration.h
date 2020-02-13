#ifndef XML_CONFIGURATION_H
#define XML_CONFIGURATION_H

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include "base_manager.h"

namespace SmilHead
{
    class XMLConfiguration : public Files::BaseManager
    {
        Q_OBJECT
    public:
        explicit XMLConfiguration(MainConfiguration *config, QObject *parent = nullptr);

        void processFromUrl(QUrl configuration_url);
        void processFromLocalFile(QString file_path);

    protected:
        Downloader        *XMLDownloader;
        QDomDocument       document;
        bool               loadDocument(QString file_path);
        QString            getAttribute(QDomElement element, QString attr_name);
        void               parse();

    protected slots:
        void doSucceed(TNetworkAccess *network);
    signals:
        void finishedConfiguration();

    };
}
#endif // XML_CONFIGURATION_H
