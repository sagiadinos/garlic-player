#ifndef INDEX_MODEL_H
#define INDEX_MODEL_H

#include <QObject>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include "logging_categories.h"

class IndexModel : public QObject
{
        Q_OBJECT
    public:
        explicit IndexModel();
        QDomElement     getHead();
        QDomElement     getBody();
        bool            loadDocument(QString file_path);
        QDomDocument getDocument() const;
        void setDocument(const QDomDocument &value);

    signals:

    protected:
        QDomDocument    document;
        QDomElement     getTag(QString tag_name);
};

#endif // INDEX_MODEL_H
