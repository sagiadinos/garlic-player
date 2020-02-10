/****************************************************************************
** Meta object code from reading C++ file 'qtav_decoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/player-widget/mm_libs/qtav_decoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtav_decoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAVDecoder_t {
    QByteArrayData data[11];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAVDecoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAVDecoder_t qt_meta_stringdata_QtAVDecoder = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QtAVDecoder"
QT_MOC_LITERAL(1, 12, 8), // "finished"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "play"
QT_MOC_LITERAL(4, 27, 4), // "stop"
QT_MOC_LITERAL(5, 32, 20), // "onMediaStatusChanged"
QT_MOC_LITERAL(6, 53, 17), // "QtAV::MediaStatus"
QT_MOC_LITERAL(7, 71, 6), // "status"
QT_MOC_LITERAL(8, 78, 19), // "displayErrorMessage"
QT_MOC_LITERAL(9, 98, 13), // "QtAV::AVError"
QT_MOC_LITERAL(10, 112, 1) // "e"

    },
    "QtAVDecoder\0finished\0\0play\0stop\0"
    "onMediaStatusChanged\0QtAV::MediaStatus\0"
    "status\0displayErrorMessage\0QtAV::AVError\0"
    "e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAVDecoder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    1,   42,    2, 0x09 /* Protected */,
       8,    1,   45,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void QtAVDecoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtAVDecoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->play(); break;
        case 2: _t->stop(); break;
        case 3: _t->onMediaStatusChanged((*reinterpret_cast< QtAV::MediaStatus(*)>(_a[1]))); break;
        case 4: _t->displayErrorMessage((*reinterpret_cast< QtAV::AVError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaStatus >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AVError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QtAVDecoder::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QtAVDecoder::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QtAVDecoder::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_QtAVDecoder.data,
    qt_meta_data_QtAVDecoder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtAVDecoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAVDecoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAVDecoder.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAVDecoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QtAVDecoder::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
