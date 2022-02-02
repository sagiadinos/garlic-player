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
        void          init(BaseMedia *media, Region *reg);
        void          deinit();
        void          changeSize(int w, int h);
        QWidget      *getView();
    protected:
        TBrush        *MyBrush;
        QScopedPointer<QLabel, QScopedPointerDeleteLater>  BrushWidget;
};

#endif // PLAYERBRUSH_H
