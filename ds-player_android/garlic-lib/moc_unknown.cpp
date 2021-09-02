/****************************************************************************
** Meta object code from reading C++ file 'unknown.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/media/unknown.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unknown.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TUnknown_t {
    QByteArrayData data[3];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TUnknown_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TUnknown_t qt_meta_stringdata_TUnknown = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TUnknown"
QT_MOC_LITERAL(1, 9, 30), // "prepareDurationTimerBeforePlay"
QT_MOC_LITERAL(2, 40, 0) // ""

    },
    "TUnknown\0prepareDurationTimerBeforePlay\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TUnknown[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void TUnknown::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TUnknown *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->prepareDurationTimerBeforePlay(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TUnknown::staticMetaObject = { {
    &BaseMedia::staticMetaObject,
    qt_meta_stringdata_TUnknown.data,
    qt_meta_data_TUnknown,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TUnknown::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TUnknown::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TUnknown.stringdata0))
        return static_cast<void*>(this);
    return BaseMedia::qt_metacast(_clname);
}

int TUnknown::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseMedia::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
