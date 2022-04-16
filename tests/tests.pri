linux {
    LIBS += -L$$PWD/../src/lib -lgarlic
    LIBS += -L/usr/lib -lgmock -lgtest
    #temporary ToDO
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -O0 -Wno-deprecated-declarations -Wno-deprecated-copy
}
macx {
    #libgarlic release is static compiled in osx
    CONFIG+=sdk_no_version_check # remove annoying SDK messages on Apple Silicon
    LIBS += -L$$PWD/../src/lib -lgarlic
}
win32 {
    Release:LIBS += -L$$shadowed($$PWD)/lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L$$shadowed($$PWD)/lib -lgarlic -lquazipd -lzlib
}
