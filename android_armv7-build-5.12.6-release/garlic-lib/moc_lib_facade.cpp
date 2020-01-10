/****************************************************************************
** Meta object code from reading C++ file 'lib_facade.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/lib_facade.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lib_facade.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LibFacade_t {
    QByteArrayData data[10];
    char stringdata0[119];
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
QT_MOC_LITERAL(6, 57, 14), // "newIndexLoaded"
QT_MOC_LITERAL(7, 72, 9), // "loadIndex"
QT_MOC_LITERAL(8, 82, 18), // "emitStartShowMedia"
QT_MOC_LITERAL(9, 101, 17) // "emitStopShowMedia"

    },
    "LibFacade\0startShowMedia\0\0BaseMedia*\0"
    "media\0stopShowMedia\0newIndexLoaded\0"
    "loadIndex\0emitStartShowMedia\0"
    "emitStopShowMedia"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LibFacade[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,
       6,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   51,    2, 0x09 /* Protected */,
       8,    1,   52,    2, 0x09 /* Protected */,
       9,    1,   55,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

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
        case 2: _t->newIndexLoaded(); break;
        case 3: _t->loadIndex(); break;
        case 4: _t->emitStartShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 5: _t->emitStopShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
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
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        case 5:
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibFacade::newIndexLoaded)) {
                *result = 2;
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
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
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
void LibFacade::newIndexLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
