
QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov

LIBS += -L../../lib -lgarlic
DESTDIR = ../../test-bin
OBJECTS_DIR = ../_compile
MOC_DIR = ../_compile
RCC_DIR = ../_compile
UI_DIR = ../_compile
