#ifndef TPRIORITYCLASS_H
#define TPRIORITYCLASS_H
#include <QQueue>
#include <QList>
#include <QStack>
#include "smilparser/playlist.h"

class TPriorityClass : public QObject
{
    Q_OBJECT
public:
    TPriorityClass(QObject * parent = 0);
    ~TPriorityClass(){}
    bool        parse(QDomElement dom_element);
    QString     getPeers(){return peers;}
    QString     getHigher(){return higher;}
    QString     getLower(){return lower;}
    bool        findElement(QDomElement dom_element);
    void        insertQueue(TBase *element);
    int         countQueue();
    TBase *     getFromQueue();
    QList<QDomElement>  getChilds();
protected:
    QList<QDomElement>            ar_playlist;
    QList<QDomElement>::iterator  iterator;
    QStack<TBase *>               ar_defer;
    QQueue<TBase *>               ar_pause;
private:
    QDomElement           root_element;
    QString               peers  = "stop";   // how elements insite a group intrerruots each other
    QString               higher = "pause";  // how an group with hier priority interrupts this group
    QString               lower  = "defer";  // how an group with lower priority interrupts this group
    int                   count_childs       = 0;
    void                  setAttributes();
    void                  setPlaylist();
};

#endif // TPRIORITYCLASS_H
