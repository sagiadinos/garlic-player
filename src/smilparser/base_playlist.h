#ifndef TPLAYLIST_H
#define TPLAYLIST_H

#include <QString>
#include <QtXml>
#include <QDebug>

struct TFoundTag
{
    QString     name;
    QDomElement dom_element;
};

/**
 * @brief The TBasePlaylist is an abstract class for Smil Playlists which is inherited by seq, par and excl.
 *        This class has functions for handling and media content. In case of media or playlist it returns the type and
 *        the actual DomElement, so that the caller can handle it
 *
 */
class TBasePlaylist : public QObject
{
    Q_OBJECT
public:
    explicit     TBasePlaylist(QObject * parent = 0);
    virtual bool parse(QDomElement element) = 0;
    virtual QString getType() = 0;
    TFoundTag    getFoundTag();
protected:
    QString      id             = "";
    QString      begin          = "0";
    QString      end            = "";
    QString      title          = "";
    int          repeatCount    = 0;
    int          internal_count = 1;
    bool         indefinite     = false;
    QDomElement  actual_element;
    TFoundTag    found_tag;
    QObject     *parent_playlist;
    void         reactByTag();
    bool         checkRepeatCountStatus();
    void         setBaseAttributes();
    void         doRef();
    void         doImage();
    void         doVideo();
    void         doAudio();
    void         doWeb();
    void         doPrefetch();
    void         doSeq();
    void         doPar();
    void         doExcl();

private:
    void setRepeatCount(QString rC);
signals:
    void        finished(QObject * , QObject *);
};

#endif // TPLAYLIST_H
