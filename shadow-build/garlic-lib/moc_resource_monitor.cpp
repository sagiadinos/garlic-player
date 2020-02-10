/****************************************************************************
** Meta object code from reading C++ file 'resource_monitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/tools/resource_monitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resource_monitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ResourceMonitor_t {
    QByteArrayData data[9];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ResourceMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ResourceMonitor_t qt_meta_stringdata_ResourceMonitor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ResourceMonitor"
QT_MOC_LITERAL(1, 16, 7), // "refresh"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 20), // "getTotalMemorySystem"
QT_MOC_LITERAL(4, 46, 19), // "getFreeMemorySystem"
QT_MOC_LITERAL(5, 66, 15), // "getMemoryAppUse"
QT_MOC_LITERAL(6, 82, 19), // "getMaxMemoryAppUsed"
QT_MOC_LITERAL(7, 102, 16), // "getThreadsNumber"
QT_MOC_LITERAL(8, 119, 19) // "getMaxThreadsNumber"

    },
    "ResourceMonitor\0refresh\0\0getTotalMemorySystem\0"
    "getFreeMemorySystem\0getMemoryAppUse\0"
    "getMaxMemoryAppUsed\0getThreadsNumber\0"
    "getMaxThreadsNumber"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ResourceMonitor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x02 /* Public */,
       3,    0,   50,    2, 0x02 /* Public */,
       4,    0,   51,    2, 0x02 /* Public */,
       5,    0,   52,    2, 0x02 /* Public */,
       6,    0,   53,    2, 0x02 /* Public */,
       7,    0,   54,    2, 0x02 /* Public */,
       8,    0,   55,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,

       0        // eod
};

void ResourceMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ResourceMonitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refresh(); break;
        case 1: { QString _r = _t->getTotalMemorySystem();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->getFreeMemorySystem();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->getMemoryAppUse();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->getMaxMemoryAppUsed();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->getThreadsNumber();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->getMaxThreadsNumber();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ResourceMonitor::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ResourceMonitor.data,
    qt_meta_data_ResourceMonitor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ResourceMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResourceMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ResourceMonitor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ResourceMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
