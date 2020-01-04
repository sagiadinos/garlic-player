#ifndef ELEMENTSCONTAINER_H
#define ELEMENTSCONTAINER_H

#include <QObject>
#include <QHash>
#include <QVector>
#include "smilparser/factory.h"

class ElementsContainer : public QObject
{
    Q_OBJECT
public:
    explicit ElementsContainer(QObject *parent = nullptr);
    ~ElementsContainer();
    BaseTimings  *findSmilElement(QDomElement dom_element);
    BaseTimings  *insertSmilElement(TContainer *parent_container, QDomElement dom_element);
    BaseMedia    *getMediaOnPosition(int position);

protected:
    QHash<QString, BaseTimings *>      all_elements_list;
    QVector<BaseMedia *>               all_presentable_media;
    void                               insertSmilMedia(BaseMedia *MyBAseMedia);

signals:

};

#endif // ELEMENTSCONTAINER_H
