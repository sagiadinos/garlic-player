#ifndef TPRIORITYCLASS_H
#define TPRIORITYCLASS_H
#include <QQueue>
#include <QList>
#include <QStack>
#include "smilparser/container.h"

class TPriorityClass : public TBase
{
    Q_OBJECT
public:
    TPriorityClass(QObject * parent = 0);
    ~TPriorityClass(){}
    bool                          parse(QDomElement dom_element);
    QString                       getPeers(){return peers;}
    QString                       getHigher(){return higher;}
    QString                       getLower(){return lower;}
    bool                          findElement(QDomElement dom_element);
    void                          insertQueue(TBaseTiming *element);
    int                           countQueue();
    TBaseTiming                  *getFromQueue();
    QList<QDomElement>            getChildList();
protected:
    QList<QDomElement>            ar_dom_childs;
    QList<QDomElement>::iterator  iterator;
    QStack<TBaseTiming *>         ar_defer;
    QQueue<TBaseTiming *>         ar_pause;
private:
    QString                       peers  = "stop";   // how elements insite a group intrerruots each other
    QString                       higher = "pause";  // how an group with hier priority interrupts this group
    QString                       lower  = "defer";  // how an group with lower priority interrupts this group
    int                           count_childs       = 0;
    void                          setAttributes();
    void                          setChildList();
};

#endif // TPRIORITYCLASS_H
