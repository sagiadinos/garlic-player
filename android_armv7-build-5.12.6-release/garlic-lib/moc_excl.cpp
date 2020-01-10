/****************************************************************************
** Meta object code from reading C++ file 'excl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/container/excl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'excl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TExcl_t {
    QByteArrayData data[8];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TExcl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TExcl_t qt_meta_stringdata_TExcl = {
    {
QT_MOC_LITERAL(0, 0, 5), // "TExcl"
QT_MOC_LITERAL(1, 6, 13), // "resumeElement"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "BaseTimings*"
QT_MOC_LITERAL(4, 34, 7), // "element"
QT_MOC_LITERAL(5, 42, 11), // "stopElement"
QT_MOC_LITERAL(6, 54, 12), // "pauseElement"
QT_MOC_LITERAL(7, 67, 26) // "setDurationTimerBeforePlay"

    },
    "TExcl\0resumeElement\0\0BaseTimings*\0"
    "element\0stopElement\0pauseElement\0"
    "setDurationTimerBeforePlay"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TExcl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void TExcl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TExcl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resumeElement((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 1: _t->stopElement((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 2: _t->pauseElement((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 3: _t->setDurationTimerBeforePlay(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TExcl::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TExcl::resumeElement)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TExcl::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TExcl::stopElement)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TExcl::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TExcl::pauseElement)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TExcl::staticMetaObject = { {
    &TContainer::staticMetaObject,
    qt_meta_stringdata_TExcl.data,
    qt_meta_data_TExcl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TExcl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TExcl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TExcl.stringdata0))
        return static_cast<void*>(this);
    return TContainer::qt_metacast(_clname);
}

int TExcl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TContainer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TExcl::resumeElement(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TExcl::stopElement(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TExcl::pauseElement(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
