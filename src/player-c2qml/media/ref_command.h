#ifndef REFCOMMAND_H
#define REFCOMMAND_H

#include "smilparser/media/ref_command.h"
#include "player_base_media.h"

class RefCommand  : public PlayerBaseMedia
{
    Q_OBJECT
public:
    explicit RefCommand(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
    ~RefCommand();

    void                        init(BaseMedia *media);
    void                        deinit();
    void                        setParentItem(QQuickItem *parent);
protected:
    QScopedPointer<QQuickItem>  ref_item;

};

#endif // REFCOMMAND_H
