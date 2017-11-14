

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
   -lgcov

LIBS += -L$$PWD/../src/lib -lgarlic -lquazip -lzlib
