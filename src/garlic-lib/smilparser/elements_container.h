#ifndef ELEMENTSCONTAINER_H
#define ELEMENTSCONTAINER_H

#include <QObject>
#include <QHash>
#include <QVector>
#include "head_parser.h"
#include "smilparser/element_factory.h"

class ElementsContainer : public QObject
{
    Q_OBJECT
public:
    explicit ElementsContainer(HeadParser *hp, QObject *parent = nullptr);
    ~ElementsContainer();
    BaseTimings  *findSmilElement(QDomElement dom_element);
    void          insertSmilElement(BaseTimings *MyBaseTimings);
    void          insertSmilMedia(BaseMedia *MyBaseMedia);
    BaseMedia    *getMediaOnZoneAndPosition(int position, int zone);

private:
    HeadParser                            *MyHeadParser;
    QHash<QString, BaseTimings *>          elements_list;
    QVector<QString>                       regions;
    QMap<QString, QVector<BaseMedia *> *>  media_list;
    BaseMedia                             *getMediaOnPosition(int position, QVector<BaseMedia *> list);
    QString                                determineZoneName(int zone);


signals:

};

#endif // ELEMENTSCONTAINER_H
