#include "elements_container.h"

ElementsContainer::ElementsContainer(QObject *parent) : QObject(parent)
{

}

ElementsContainer::~ElementsContainer()
{
    qDeleteAll(all_elements_list);
    all_elements_list.clear();
}

BaseTimings *ElementsContainer::findSmilElement(QDomElement dom_element)
{
    BaseTimings                             *MyBaseTimings        = Q_NULLPTR;
    QString                                  name                 = dom_element.nodeName();
    QHash<QString, BaseTimings *>::iterator  ar_elements_iterator = all_elements_list.find(TBase::parseID(dom_element));
    if (ar_elements_iterator != all_elements_list.end())
    {
        MyBaseTimings = *ar_elements_iterator;
    }
    return MyBaseTimings;
}

BaseTimings *ElementsContainer::insertSmilElement(TContainer *parent_container, QDomElement dom_element)
{
    BaseTimings                             *MyBaseTimings         = Q_NULLPTR;
    MyBaseTimings     = TFactory::createBase(dom_element, parent_container, this);
    MyBaseTimings->parse(dom_element);
    all_elements_list.insert(MyBaseTimings->getID(), MyBaseTimings);

    return MyBaseTimings;
}
