include(../defaults.pri)
INCLUDEPATH += ../garlic-lib
INCLUDEPATH += ../garlic-lib/smilparser

QT += quick network xml
CONFIG += warn_on  c++11

#Delete or comment out the next line when you want to use QTMultimedia
#CONFIG    += support_qtav
support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += av
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QUAZIP_STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    backend.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!android {
    QT += webengine
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
android {
    QT += androidextras
    LIBS += -L../libandroid -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

HEADERS += \
    backend.h
