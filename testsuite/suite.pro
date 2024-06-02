include(../src/defaults.pri)

QT += widgets xml testlib

SOURCES = main.cpp \
    base_testsuite.cpp \
    garlic-lib/tools/log_file/tst_log_file.cpp \
    garlic-lib/tools/reboot/weekdayconverter/tst_weekdayconverter.cpp

HEADERS += \
    base_testsuite.hpp \
    garlic-lib/tools/log_file/tst_log_file.hpp \
    garlic-lib/tools/reboot/weekdayconverter/tst_weekdayconverter.hpp

linux {
    LIBS += -L$$PWD/../src/lib -lgarlic
    LIBS += -L/usr/lib -lgmock -lgtest
    #temporary ToDO
    #  warning: template-id not allowed for constructor in C++20 in Arch gcc
    QMAKE_CXXFLAGS += -Wno-template-id-cdtor
}
