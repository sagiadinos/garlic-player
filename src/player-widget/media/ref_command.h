#ifndef REFCOMMAND_H
#define REFCOMMAND_H

#include "player_base_media.h"
#include "smilparser/media/ref_command.h"

class PlayerRefCommand : public PlayerBaseMedia
{
    Q_OBJECT
public:
    explicit PlayerRefCommand(QObject *parent = nullptr);
    ~PlayerRefCommand();
    void          loadMedia(BaseMedia *media, Region *reg);
    void          play();
    void          stop();
    void          resume();
    void          pause();
    void          changeSize(int w, int h);
    QWidget      *getView();

signals:

};

#endif // REFCOMMAND_H
