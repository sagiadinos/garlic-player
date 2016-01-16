#ifndef TPRIORITYCLASS_H
#define TPRIORITYCLASS_H
#include <QQueue>
#include <QHash>
#include "smilparser/playlist.h"

class TPriorityClass : public TPlaylist
{
    Q_OBJECT
public:
    TPriorityClass(QObject * parent = 0);
    ~TPriorityClass(){}
    bool        parse(QDomElement element);
    void        beginPlay();
    void        endPlay(){}
    bool        next();
    bool        previous();
    QString     getType(){return "TExcl";} // priorityClass elements may only appear as immediate children of excl elements
    QString     getPeers(){return peers;}
    QString     getHigher(){return higher;}
    QString     getLower(){return lower;}
    bool        findElement(QDomElement element);
public slots:
    void      play();
private:
    QDomElement           active_element, tmp_active_element;
    QQueue<QDomElement>   pause_queue;
    QString               peers  = "stop";   // how elements insite a group intrerruots each other
    QString               higher = "pause";  // how an group with hier priority interrupts this group
    QString               lower  = "defer";  // how an group with lower priority interrupts this group
    int                   count_childs       = 0;
    void      setPlaylist();
    void      setAttributes();


};

#endif // TPRIORITYCLASS_H
