/****************************************************************************
** Meta object code from reading C++ file 'base_media.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/media/base_media.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'base_media.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseMedia_t {
    QByteArrayData data[11];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseMedia_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseMedia_t qt_meta_stringdata_BaseMedia = {
    {
QT_MOC_LITERAL(0, 0, 9), // "BaseMedia"
QT_MOC_LITERAL(1, 10, 12), // "startedMedia"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "TContainer*"
QT_MOC_LITERAL(4, 36, 6), // "parent"
QT_MOC_LITERAL(5, 43, 12), // "BaseTimings*"
QT_MOC_LITERAL(6, 56, 7), // "element"
QT_MOC_LITERAL(7, 64, 13), // "finishedMedia"
QT_MOC_LITERAL(8, 78, 14), // "preloadElement"
QT_MOC_LITERAL(9, 93, 11), // "QDomElement"
QT_MOC_LITERAL(10, 105, 12) // "emitfinished"

    },
    "BaseMedia\0startedMedia\0\0TContainer*\0"
    "parent\0BaseTimings*\0element\0finishedMedia\0"
    "preloadElement\0QDomElement\0emitfinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseMedia[] = {

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
       1,    2,   34,    2, 0x06 /* Public */,
       7,    2,   39,    2, 0x06 /* Public */,
       8,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 9,    4,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void BaseMedia::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BaseMedia *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startedMedia((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< BaseTimings*(*)>(_a[2]))); break;
        case 1: _t->finishedMedia((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< BaseTimings*(*)>(_a[2]))); break;
        case 2: _t->preloadElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< QDomElement(*)>(_a[2]))); break;
        case 3: _t->emitfinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
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
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BaseMedia::*)(TContainer * , BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseMedia::startedMedia)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BaseMedia::*)(TContainer * , BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseMedia::finishedMedia)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BaseMedia::*)(TContainer * , QDomElement );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseMedia::preloadElement)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BaseMedia::staticMetaObject = { {
    &BaseTimings::staticMetaObject,
    qt_meta_stringdata_BaseMedia.data,
    qt_meta_data_BaseMedia,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BaseMedia::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseMedia::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaseMedia.stringdata0))
        return static_cast<void*>(this);
    return BaseTimings::qt_metacast(_clname);
}

int BaseMedia::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseTimings::qt_metacall(_c, _id, _a);
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
void BaseMedia::startedMedia(TContainer * _t1, BaseTimings * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BaseMedia::finishedMedia(TContainer * _t1, BaseTimings * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BaseMedia::preloadElement(TContainer * _t1, QDomElement _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
