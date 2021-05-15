#ifndef ELEMENTSCONTAINER_H
#define ELEMENTSCONTAINER_H

#include <QObject>
#include <QHash>
#include "base_timings.h"

class ElementsContainer : public QObject
{
    Q_OBJECT
public:
    explicit ElementsContainer(QObject *parent = nullptr);
    ~ElementsContainer();
    BaseTimings  *findSmilElementByDom(QDomElement dom_element);
    BaseTimings  *findSmilElementById(QString id);
    void          insertSmilElement(BaseTimings *MyBaseTimings);
    void          distributeTriggers();

private:
    QHash<QString, BaseTimings *>          elements_list;
    void  distributeBeginTrigger(BaseTimings *bt_target );
    void  distributeEndTrigger(BaseTimings *bt_target );

signals:

};

#endif // ELEMENTSCONTAINER_H
