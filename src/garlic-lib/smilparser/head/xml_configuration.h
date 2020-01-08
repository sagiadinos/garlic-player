#ifndef XML_CONFIGURATION_H
#define XML_CONFIGURATION_H

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include "configuration.h"
#include "base_manager.h"

namespace SmilHead
{
    class XMLConfiguration : public BaseManager
    {
        Q_OBJECT
    public:
        explicit XMLConfiguration(TConfiguration *config, QObject *parent = nullptr);

        void init(QUrl configuration_url);

    protected:
        TConfiguration *MyConfiguration;
        Downloader     *XMLDownloader;
        QDomDocument    document;
        bool loadDocument(QString file_path);
        QString getAttribute(QDomElement element, QString attr_name);

    protected slots:
        void doSucceedParse(TNetworkAccess *network);
    signals:
        void finishedConfiguration();

    };
}
#endif // XML_CONFIGURATION_H
