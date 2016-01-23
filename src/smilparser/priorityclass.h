#ifndef TPRIORITYCLASS_H
#define TPRIORITYCLASS_H
#include <QQueue>
#include <QHash>
#include "smilparser/playlist.h"

class TPriorityClass : public QObject
{
    Q_OBJECT
public:
    TPriorityClass(QObject * parent = 0);
    ~TPriorityClass(){}
    bool        parse(QDomElement dom_element);
    void        stopPlayingChild(QDomElement new_dom_element);
    void        pausePlayingChild(QDomElement new_dom_element);
    void        pauseElement(QObject *element);
    void        enqueueElement(QObject *element);
    QString     getPeers(){return peers;}
    QString     getHigher(){return higher;}
    QString     getLower(){return lower;}
    bool        findElement(QDomElement dom_element);
    QDomElement getActiveElement(){return active_element;}
    void        setActiveChilds(bool active);
    bool        isChildActive(){return is_child_active;}
    int         decPlayableChilds();
    QList<QDomElement>  getChilds();
    bool play();
    bool next();
protected:
    QList<QDomElement>            ar_playlist;
    QList<QDomElement>::iterator            iterator;
private:
    QDomElement           root_element, active_element;
    QQueue<QObject *>     queue;
    QString               peers  = "stop";   // how elements insite a group intrerruots each other
    QString               higher = "pause";  // how an group with hier priority interrupts this group
    QString               lower  = "defer";  // how an group with lower priority interrupts this group
    int                   count_childs       = 0;
    int                   playable_childs    = 0;
    bool                  is_child_active    = false;
    void                  setAttributes();
    void                  setPlaylist();
};

#endif // TPRIORITYCLASS_H
