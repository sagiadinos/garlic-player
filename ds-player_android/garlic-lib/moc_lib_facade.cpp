/****************************************************************************
** Meta object code from reading C++ file 'lib_facade.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/lib_facade.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lib_facade.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LibFacade_t {
    QByteArrayData data[21];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LibFacade_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LibFacade_t qt_meta_stringdata_LibFacade = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LibFacade"
QT_MOC_LITERAL(1, 10, 14), // "startShowMedia"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "BaseMedia*"
QT_MOC_LITERAL(4, 37, 5), // "media"
QT_MOC_LITERAL(5, 43, 13), // "stopShowMedia"
QT_MOC_LITERAL(6, 57, 15), // "readyForPlaying"
QT_MOC_LITERAL(7, 73, 9), // "newConfig"
QT_MOC_LITERAL(8, 83, 8), // "rebootOS"
QT_MOC_LITERAL(9, 92, 7), // "task_id"
QT_MOC_LITERAL(10, 100, 15), // "installSoftware"
QT_MOC_LITERAL(11, 116, 9), // "file_path"
QT_MOC_LITERAL(12, 126, 10), // "initParser"
QT_MOC_LITERAL(13, 137, 9), // "loadIndex"
QT_MOC_LITERAL(14, 147, 12), // "changeConfig"
QT_MOC_LITERAL(15, 160, 19), // "emitInstallSoftware"
QT_MOC_LITERAL(16, 180, 6), // "reboot"
QT_MOC_LITERAL(17, 187, 18), // "emitStartShowMedia"
QT_MOC_LITERAL(18, 206, 17), // "emitStopShowMedia"
QT_MOC_LITERAL(19, 224, 18), // "processBodyParsing"
QT_MOC_LITERAL(20, 243, 18) // "preparedForPlaying"

    },
    "LibFacade\0startShowMedia\0\0BaseMedia*\0"
    "media\0stopShowMedia\0readyForPlaying\0"
    "newConfig\0rebootOS\0task_id\0installSoftware\0"
    "file_path\0initParser\0loadIndex\0"
    "changeConfig\0emitInstallSoftware\0"
    "reboot\0emitStartShowMedia\0emitStopShowMedia\0"
    "processBodyParsing\0preparedForPlaying"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LibFacade[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       5,    1,   92,    2, 0x06 /* Public */,
       6,    0,   95,    2, 0x06 /* Public */,
       7,    0,   96,    2, 0x06 /* Public */,
       8,    1,   97,    2, 0x06 /* Public */,
      10,    1,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  103,    2, 0x0a /* Public */,
      13,    0,  104,    2, 0x09 /* Protected */,
      14,    0,  105,    2, 0x09 /* Protected */,
      15,    1,  106,    2, 0x09 /* Protected */,
      16,    1,  109,    2, 0x09 /* Protected */,
      17,    1,  112,    2, 0x09 /* Protected */,
      18,    1,  115,    2, 0x09 /* Protected */,
      19,    0,  118,    2, 0x09 /* Protected */,
      20,    0,  119,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LibFacade::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LibFacade *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 1: _t->stopShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 2: _t->readyForPlaying(); break;
        case 3: _t->newConfig(); break;
        case 4: _t->rebootOS((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->installSoftware((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->initParser(); break;
        case 7: _t->loadIndex(); break;
        case 8: _t->changeConfig(); break;
        case 9: _t->emitInstallSoftware((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->reboot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->emitStartShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 12: _t->emitStopShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 13: _t->processBodyParsing(); break;
        case 14: _t->preparedForPlaying(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LibFacade::*)(BaseMedia * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::startShowMedia)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LibFacade::*)(BaseMedia * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::stopShowMedia)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LibFacade::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::readyForPlaying)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LibFacade::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::newConfig)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LibFacade::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::rebootOS)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LibFacade::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::installSoftware)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LibFacade::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_LibFacade.data,
    qt_meta_data_LibFacade,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LibFacade::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LibFacade::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LibFacade.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LibFacade::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void LibFacade::startShowMedia(BaseMedia * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LibFacade::stopShowMedia(BaseMedia * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LibFacade::readyForPlaying()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LibFacade::newConfig()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void LibFacade::rebootOS(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LibFacade::installSoftware(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
