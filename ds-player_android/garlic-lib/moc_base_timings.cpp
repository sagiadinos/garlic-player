/****************************************************************************
** Meta object code from reading C++ file 'base_timings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/base_timings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'base_timings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseTimings_t {
    QByteArrayData data[15];
    char stringdata0[296];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseTimings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseTimings_t qt_meta_stringdata_BaseTimings = {
    {
QT_MOC_LITERAL(0, 0, 11), // "BaseTimings"
QT_MOC_LITERAL(1, 12, 26), // "finishActiveDurationSignal"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 12), // "BaseTimings*"
QT_MOC_LITERAL(4, 53, 26), // "finishSimpleDurationSignal"
QT_MOC_LITERAL(5, 80, 18), // "triggerBeginSignal"
QT_MOC_LITERAL(6, 99, 16), // "triggerEndSignal"
QT_MOC_LITERAL(7, 116, 18), // "startElementSignal"
QT_MOC_LITERAL(8, 135, 17), // "stopElementSignal"
QT_MOC_LITERAL(9, 153, 19), // "resumeElementSignal"
QT_MOC_LITERAL(10, 173, 18), // "pauseElementSignal"
QT_MOC_LITERAL(11, 192, 26), // "emitfinishedActiveDuration"
QT_MOC_LITERAL(12, 219, 30), // "prepareDurationTimerBeforePlay"
QT_MOC_LITERAL(13, 250, 22), // "finishedSimpleDuration"
QT_MOC_LITERAL(14, 273, 22) // "finishedActiveDuration"

    },
    "BaseTimings\0finishActiveDurationSignal\0"
    "\0BaseTimings*\0finishSimpleDurationSignal\0"
    "triggerBeginSignal\0triggerEndSignal\0"
    "startElementSignal\0stopElementSignal\0"
    "resumeElementSignal\0pauseElementSignal\0"
    "emitfinishedActiveDuration\0"
    "prepareDurationTimerBeforePlay\0"
    "finishedSimpleDuration\0finishedActiveDuration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseTimings[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       5,    2,   80,    2, 0x06 /* Public */,
       6,    2,   85,    2, 0x06 /* Public */,
       7,    1,   90,    2, 0x06 /* Public */,
       8,    1,   93,    2, 0x06 /* Public */,
       9,    1,   96,    2, 0x06 /* Public */,
      10,    1,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  102,    2, 0x0a /* Public */,
      12,    0,  103,    2, 0x0a /* Public */,
      13,    0,  104,    2, 0x0a /* Public */,
      14,    0,  105,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
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
        case 0: _t->finishActiveDurationSignal((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 1: _t->finishSimpleDurationSignal((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 2: _t->triggerBeginSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->triggerEndSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->startElementSignal((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 5: _t->stopElementSignal((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 6: _t->resumeElementSignal((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 7: _t->pauseElementSignal((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 8: _t->emitfinishedActiveDuration(); break;
        case 9: _t->prepareDurationTimerBeforePlay(); break;
        case 10: _t->finishedSimpleDuration(); break;
        case 11: _t->finishedActiveDuration(); break;
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
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 7:
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
            using _t = void (BaseTimings::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::finishActiveDurationSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::finishSimpleDurationSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::triggerBeginSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::triggerEndSignal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::startElementSignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::stopElementSignal)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::resumeElementSignal)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (BaseTimings::*)(BaseTimings * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseTimings::pauseElementSignal)) {
                *result = 7;
                return;
            }
        }
    }
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void BaseTimings::finishActiveDurationSignal(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BaseTimings::finishSimpleDurationSignal(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BaseTimings::triggerBeginSignal(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BaseTimings::triggerEndSignal(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BaseTimings::startElementSignal(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BaseTimings::stopElementSignal(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BaseTimings::resumeElementSignal(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void BaseTimings::pauseElementSignal(BaseTimings * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
