#include "index_model.h"

IndexModel::IndexModel(QObject *parent) : QObject(parent)
{

}

QDomElement IndexModel::getHead()
{
    return getTag("head");
}

QDomElement IndexModel::getBody()
{
    return getTag("body");
}

bool IndexModel::loadDocument(QString file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical(ContentManager) << "SMIL_OPEN_ERROR resourceUri:" << file_path << "Fail to load as SMIL index";
        return false;
    }
    if (!document.setContent(&file))
    {
        qCritical(ContentManager) << "SMIL_OPEN_ERROR resourceUri:" << file_path << "SMIL index syntax error or other";
        file.close();
        return false;
    }
    file.close();
    return true;
}

QDomDocument IndexModel::getDocument() const
{
    return document;
}

void IndexModel::setDocument(const QDomDocument &value)
{
    document = value;
}

// ==================  protected methods =======================================

QDomElement IndexModel::getTag(QString tag_name)
{
    QDomNodeList nodelist = document.elementsByTagName(tag_name);
    if (nodelist.length() == 0) // fallback for ensure giving an empty named Tag-element back
        return document.createElement(tag_name);

    return nodelist.item(0).toElement();
}
