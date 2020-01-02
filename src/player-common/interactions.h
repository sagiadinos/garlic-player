#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include <QObject>
#include <QKeyEvent>
#include "lib_facade.h"

class Interactions : public QObject
{
    Q_OBJECT
public:
    explicit Interactions(LibFacade *libfacade, QObject *parent = nullptr);
    void     handleKeyPress(QKeyEvent *KeyEvent);
protected:
    QString  key_string = "";
    QString  zone = "";
    QString  position = "";
    LibFacade *MyLibFacade;
private:
    void setZone();
    void resetKeyString();
    void resetAll();
signals:

};

#endif // INTERACTIONS_H
