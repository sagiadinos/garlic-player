#include "elements_container.h"

ElementsContainer::ElementsContainer(QObject *parent) : QObject(parent)
{

}

ElementsContainer::~ElementsContainer()
{
    qDeleteAll(all_elements_list);
    // checking if qDeleteAll crashs
/*    for(QHash<QString, BaseTimings *>::iterator  i = all_elements_list.begin(); i != all_elements_list.end(); i++)
    {
        BaseTimings                             *MyBaseTimings     = Q_NULLPTR;
        MyBaseTimings = *i;
        delete MyBaseTimings;
    }
*/
    all_elements_list.clear();

    // not neccesary do delete here cause same objects are in all_elements_list
    all_presentable_media.clear();
}

BaseTimings *ElementsContainer::findSmilElement(QDomElement dom_element)
{
    BaseTimings                             *MyBaseTimings     = Q_NULLPTR;
    QString                                  name              = dom_element.nodeName();
    QHash<QString, BaseTimings *>::iterator  elements_iterator = all_elements_list.find(TBase::parseID(dom_element));
    if (elements_iterator != all_elements_list.end())
    {
        MyBaseTimings = *elements_iterator;
    }
    return MyBaseTimings;
}

BaseTimings *ElementsContainer::insertSmilElement(TContainer *parent_container, QDomElement dom_element)
{
    BaseTimings *MyBaseTimings = TFactory::createBase(dom_element, parent_container, this);
    MyBaseTimings->parse(dom_element);

    all_elements_list.insert(MyBaseTimings->getID(), MyBaseTimings);

    if (MyBaseTimings->getBaseType() == "media")
    {
        insertSmilMedia(qobject_cast<BaseMedia *> (MyBaseTimings));
    }

    return MyBaseTimings;
}

BaseMedia *ElementsContainer::getMediaOnPosition(int position)
{
    int array_position = position - 1;

    if (array_position < 0)
    {
        return all_presentable_media.last();
    }
    if (array_position > all_presentable_media.size())
    {
        return all_presentable_media.first();
    }
    return all_presentable_media.at(array_position);
}

void ElementsContainer::insertSmilMedia(BaseMedia *MyBaseMedia)
{
    if (MyBaseMedia->isPresentable())
    {
        all_presentable_media.append(MyBaseMedia);
    }

}
