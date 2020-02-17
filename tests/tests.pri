unix{
    LIBS += -L$$PWD/../src/lib -lgarlic
    #temporary ToDO
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy

}
win32 {
    Release:LIBS += -L$$shadowed($$PWD)/lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L$$shadowed($$PWD)/lib -lgarlic -lquazipd -lzlib
}
