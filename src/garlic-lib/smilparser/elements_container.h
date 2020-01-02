#ifndef ELEMENTSCONTAINER_H
#define ELEMENTSCONTAINER_H

#include <QObject>
#include "smilparser/factory.h"

class ElementsContainer : public QObject
{
    Q_OBJECT
public:
    explicit ElementsContainer(QObject *parent = nullptr);
    ~ElementsContainer();
    BaseTimings  *findSmilElement(QDomElement dom_element);
    BaseTimings  *insertSmilElement(TContainer *parent_container, QDomElement dom_element);


protected:
    QHash<QString, BaseTimings *>      all_elements_list;

signals:

};

#endif // ELEMENTSCONTAINER_H
