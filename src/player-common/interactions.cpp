#include "interactions.h"

Interactions::Interactions(LibFacade *libfacade, QObject *parent) :  QObject(parent), MyLibFacade(libfacade)
{

}

void Interactions::handleKeyPress(QKeyEvent *KeyEvent)
{
    switch (KeyEvent->key())
    {
        case Qt::Key_Return:
            break;
        case Qt::Key_Right:
            MyLibFacade->playNextSmilElement();
            break;
        case Qt::Key_Left:
            MyLibFacade->playPreviousSmilElement();
            break;
        case Qt::Key_Tab:
            setZone();
            break;
        case Qt::Key_Escape:
            resetAll();
            break;
        default:
            key_string.append(KeyEvent->text());
            break;
    }
    return;
}

void Interactions::setZone()
{
    zone = key_string;
    resetKeyString();
}

void Interactions::resetAll()
{
    zone = "";
    resetKeyString();
}

void Interactions::resetKeyString()
{
    key_string = "";
}
