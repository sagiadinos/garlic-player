SOURCES += \
    ../player-common/cmdparser.cpp \
    ../player-common/debug_infos.cpp \
    ../player-common/configdialog.cpp \
    ../player-common/screen.cpp

HEADERS  += \
    ../player-common/cmdparser.h \
    ../player-common/configdialog.h \
    ../player-common/debug_infos.h \
    ../player-common/screen.h

FORMS += \
    ../player-common/configdialog.ui \
    ../player-common/debug_infos.ui

unix:!android {
    SOURCES += ../player-common/network_dialog.cpp
    HEADERS += ../player-common/network_dialog.h
    FORMS   +=  ../player-common/networkdialog.ui
}
