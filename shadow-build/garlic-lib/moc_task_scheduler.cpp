/****************************************************************************
** Meta object code from reading C++ file 'task_scheduler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/head/task_scheduler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'task_scheduler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmilHead__TaskScheduler_t {
    QByteArrayData data[12];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SmilHead__TaskScheduler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SmilHead__TaskScheduler_t qt_meta_stringdata_SmilHead__TaskScheduler = {
    {
QT_MOC_LITERAL(0, 0, 23), // "SmilHead::TaskScheduler"
QT_MOC_LITERAL(1, 24, 18), // "applyConfiguration"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 15), // "installSoftware"
QT_MOC_LITERAL(4, 60, 9), // "file_path"
QT_MOC_LITERAL(5, 70, 6), // "reboot"
QT_MOC_LITERAL(6, 77, 9), // "doSucceed"
QT_MOC_LITERAL(7, 87, 15), // "TNetworkAccess*"
QT_MOC_LITERAL(8, 103, 7), // "network"
QT_MOC_LITERAL(9, 111, 22), // "emitApplyConfiguration"
QT_MOC_LITERAL(10, 134, 19), // "emitInstallSoftware"
QT_MOC_LITERAL(11, 154, 18) // "emitShutdownPlayer"

    },
    "SmilHead::TaskScheduler\0applyConfiguration\0"
    "\0installSoftware\0file_path\0reboot\0"
    "doSucceed\0TNetworkAccess*\0network\0"
    "emitApplyConfiguration\0emitInstallSoftware\0"
    "emitShutdownPlayer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmilHead__TaskScheduler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,
       5,    0,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   54,    2, 0x09 /* Protected */,
       9,    0,   57,    2, 0x09 /* Protected */,
      10,    1,   58,    2, 0x09 /* Protected */,
      11,    0,   61,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

       0        // eod
};

void SmilHead::TaskScheduler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TaskScheduler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->applyConfiguration(); break;
        case 1: _t->installSoftware((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->reboot(); break;
        case 3: _t->doSucceed((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        case 4: _t->emitApplyConfiguration(); break;
        case 5: _t->emitInstallSoftware((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->emitShutdownPlayer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TNetworkAccess* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TaskScheduler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskScheduler::applyConfiguration)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TaskScheduler::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskScheduler::installSoftware)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TaskScheduler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskScheduler::reboot)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SmilHead::TaskScheduler::staticMetaObject = { {
    &Files::BaseManager::staticMetaObject,
    qt_meta_stringdata_SmilHead__TaskScheduler.data,
    qt_meta_data_SmilHead__TaskScheduler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SmilHead::TaskScheduler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmilHead::TaskScheduler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmilHead__TaskScheduler.stringdata0))
        return static_cast<void*>(this);
    return Files::BaseManager::qt_metacast(_clname);
}

int SmilHead::TaskScheduler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Files::BaseManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SmilHead::TaskScheduler::applyConfiguration()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SmilHead::TaskScheduler::installSoftware(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SmilHead::TaskScheduler::reboot()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
