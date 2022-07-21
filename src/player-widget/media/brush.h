#ifndef PLAYERBRUSH_H
#define PLAYERBRUSH_H

#include <QLabel>
#include "player_base_media.h"
#include "smilparser/media/brush.h"

class PlayerBrush : public PlayerBaseMedia
{
        Q_OBJECT
    public:
        PlayerBrush(QObject *parent);
        ~PlayerBrush();
        void          loadMedia(BaseMedia *media, Region *reg);
        void          play();
        void          stop();
        void          resume();
        void          pause();
        void          changeSize(int w, int h);
        QWidget      *getView();
    protected:
        TBrush        *MyBrush;
        QScopedPointer<QLabel, QScopedPointerDeleteLater>  BrushWidget;
};

#endif // PLAYERBRUSH_H
