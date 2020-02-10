/****************************************************************************
** Meta object code from reading C++ file 'container.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/container/container.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'container.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TContainer_t {
    QByteArrayData data[11];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TContainer_t qt_meta_stringdata_TContainer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TContainer"
QT_MOC_LITERAL(1, 11, 12), // "foundElement"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "TContainer*"
QT_MOC_LITERAL(4, 37, 11), // "QDomElement"
QT_MOC_LITERAL(5, 49, 16), // "startedContainer"
QT_MOC_LITERAL(6, 66, 12), // "BaseTimings*"
QT_MOC_LITERAL(7, 79, 17), // "finishedContainer"
QT_MOC_LITERAL(8, 97, 14), // "preloadElement"
QT_MOC_LITERAL(9, 112, 6), // "parent"
QT_MOC_LITERAL(10, 119, 12) // "emitfinished"

    },
    "TContainer\0foundElement\0\0TContainer*\0"
    "QDomElement\0startedContainer\0BaseTimings*\0"
    "finishedContainer\0preloadElement\0"
    "parent\0emitfinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    2,   44,    2, 0x06 /* Public */,
       7,    2,   49,    2, 0x06 /* Public */,
       8,    2,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6,    2,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 6,    2,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    9,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void TContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TContainer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->foundElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< QDomElement(*)>(_a[2]))); break;
        case 1: _t->startedContainer((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< BaseTimings*(*)>(_a[2]))); break;
        case 2: _t->finishedContainer((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< BaseTimings*(*)>(_a[2]))); break;
        case 3: _t->preloadElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< QDomElement(*)>(_a[2]))); break;
        case 4: _t->emitfinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TContainer::*)(TContainer * , QDomElement );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TContainer::foundElement)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TContainer::*)(TContainer * , BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TContainer::startedContainer)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TContainer::*)(TContainer * , BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TContainer::finishedContainer)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TContainer::*)(TContainer * , QDomElement );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TContainer::preloadElement)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TContainer::staticMetaObject = { {
    &BaseTimings::staticMetaObject,
    qt_meta_stringdata_TContainer.data,
    qt_meta_data_TContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TContainer.stringdata0))
        return static_cast<void*>(this);
    return BaseTimings::qt_metacast(_clname);
}

int TContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseTimings::qt_metacall(_c, _id, _a);
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
void TContainer::foundElement(TContainer * _t1, QDomElement _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TContainer::startedContainer(TContainer * _t1, BaseTimings * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TContainer::finishedContainer(TContainer * _t1, BaseTimings * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TContainer::preloadElement(TContainer * _t1, QDomElement _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
