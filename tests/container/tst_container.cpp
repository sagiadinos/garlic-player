#include <QString>
#include <QtTest>

#include "smilparser/container.h"

class InhertitedTContainer : public TContainer
{
    Q_OBJECT
public:
    InhertitedTContainer(TBase * parent = 0){Q_UNUSED(parent);setObjectName("testBasePlaylist");}
    QString       getType(){return "test base playlist";}

    bool          parse(QDomElement element){Q_UNUSED(element);return true;}                // virtual
    void          childEnded(TBase *element){ Q_UNUSED(element);}                           // virtual
    void          play(){}                                                                  // virtual
    void          pause(){}                                                                 // virtual
    void          stop(){}                                                                  // virtual
    void          resume(){}                                                                // virtual
    TBaseTiming  *getChildElementFromList(){ return this;}                                  // virtual
    bool          isChildPlayable(TBaseTiming *element){Q_UNUSED(element); return true;}    // virtual
    void          next(TBaseTiming *element){Q_UNUSED(element);}                            // virtual

    // for testing
    QString   test_reactByTag(){return reactByTag();}
    void      setActiveElement(QDomElement element){active_element = element;}
public slots:
    void      emitfinished(){}                                                              // virtual
protected:
    void      setDurationTimerBeforePlay(){}                                                // virtual
};


class TestTContainer : public QObject
{
    Q_OBJECT

public:
    TestTContainer(){}

private Q_SLOTS:
    void test_reactByTag();
    void test_reactByTagForRef();
};


void TestTContainer::test_reactByTag()
{
    InhertitedTContainer MyPlaylist;
    QDomDocument document;
    QDomElement  element = document.createElement(QString("img"));

    MyPlaylist.setActiveElement(element);
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("img"));

    element.setTagName("video");
    MyPlaylist.setActiveElement(element);
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("video"));

    element.setTagName("audio");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("audio"));

    element.setTagName("text");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("text"));

    element.setTagName("seq");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("seq"));

    element.setTagName("par");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("par"));

    element.setTagName("excl");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("excl"));
    return;
}

void TestTContainer::test_reactByTagForRef()
{
    InhertitedTContainer MyPlaylist;
    QDomDocument document;
    QDomElement element = document.createElement(QString("ref"));
    QCOMPARE(element.tagName(), QString("ref"));

    element.setAttribute("type", "image/png");
    MyPlaylist.setActiveElement(element);
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("img"));

    element.setAttribute("type", "video/mp4");
    MyPlaylist.setActiveElement(element);
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("video"));

    element.setAttribute("type", "audio/wav");
    MyPlaylist.setActiveElement(element);
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("audio"));

    element.setAttribute("type", "text/html");
    MyPlaylist.setActiveElement(element);
    QCOMPARE(MyPlaylist.test_reactByTag(), QString("text"));
    return;
}


QTEST_APPLESS_MAIN(TestTContainer)

#include "tst_container.moc"
