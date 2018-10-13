unix{
    LIBS += -L$$shadowed($$PWD)/lib  -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L$$shadowed($$PWD)/lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L$$shadowed($$PWD)/lib -lgarlic -lquazipd -lzlib
}
