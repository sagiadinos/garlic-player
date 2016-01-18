#include "test_smil_image.h"


void Test_SmilImage::test_beginValues()
{


    QDomDocument document;

    document.setContent(QString("<img src=\"data/images/0001.jpg\" begin=\"0s\" dur=\"1s\"/>"));
    QDomElement element = document.firstChild().toElement();

    Test_SmilImage MyImage;
    connect(MyImage, SIGNAL(startedMedia(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
    connect(MyImage, SIGNAL(finishedMedia(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));

//    document.setContent(QString("<img src=\"data/images/0001.jpg\" begin=\"1s\" dur=\"1s\" end=\"3s\"/>"));

//      document.setContent(QString("<img src=\"data/images/0001.jpg\" min=\"2s\" begin=\"0s\" dur=\"1s\" end=\"3s\"/>"));

//      document.setContent(QString("<img src=\"data/images/0001.jpg\" max=\"2s\" begin=\"0s\" dur=\"1s\" end=\"3s\"/>"));

}
