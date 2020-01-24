/****************************************************************************
** Meta object code from reading C++ file 'base_timings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/base_timings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'base_timings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseTimings_t {
    QByteArrayData data[7];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseTimings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseTimings_t qt_meta_stringdata_BaseTimings = {
    {
QT_MOC_LITERAL(0, 0, 11), // "BaseTimings"
QT_MOC_LITERAL(1, 12, 12), // "emitfinished"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 22), // "finishedSimpleDuration"
QT_MOC_LITERAL(4, 49, 26), // "setDurationTimerBeforePlay"
QT_MOC_LITERAL(5, 76, 11), // "releasePlay"
QT_MOC_LITERAL(6, 88, 22) // "finishedActiveDuration"

    },
    "BaseTimings\0emitfinished\0\0"
    "finishedSimpleDuration\0"
    "setDurationTimerBeforePlay\0releasePlay\0"
    "finishedActiveDuration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseTimings[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x09 /* Protected */,
       6,    0,   43,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BaseTimings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BaseTimings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitfinished(); break;
        case 1: _t->finishedSimpleDuration(); break;
        case 2: _t->setDurationTimerBeforePlay(); break;
        case 3: _t->releasePlay(); break;
        case 4: _t->finishedActiveDuration(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject BaseTimings::staticMetaObject = { {
    &TBase::staticMetaObject,
    qt_meta_stringdata_BaseTimings.data,
    qt_meta_data_BaseTimings,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BaseTimings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseTimings::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaseTimings.stringdata0))
        return static_cast<void*>(this);
    return TBase::qt_metacast(_clname);
}

int BaseTimings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
