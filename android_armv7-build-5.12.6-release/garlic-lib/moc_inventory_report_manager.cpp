/****************************************************************************
** Meta object code from reading C++ file 'inventory_report_manager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/reports/inventory_report_manager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inventory_report_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Reporting__InventoryReportManager_t {
    QByteArrayData data[6];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Reporting__InventoryReportManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Reporting__InventoryReportManager_t qt_meta_stringdata_Reporting__InventoryReportManager = {
    {
QT_MOC_LITERAL(0, 0, 33), // "Reporting::InventoryReportMan..."
QT_MOC_LITERAL(1, 34, 9), // "doSucceed"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 15), // "TNetworkAccess*"
QT_MOC_LITERAL(4, 61, 8), // "uploader"
QT_MOC_LITERAL(5, 70, 8) // "doFailed"

    },
    "Reporting::InventoryReportManager\0"
    "doSucceed\0\0TNetworkAccess*\0uploader\0"
    "doFailed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Reporting__InventoryReportManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x09 /* Protected */,
       5,    1,   27,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void Reporting::InventoryReportManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InventoryReportManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doSucceed((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        case 1: _t->doFailed((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TNetworkAccess* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TNetworkAccess* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Reporting::InventoryReportManager::staticMetaObject = { {
    &Reporting::BaseReportManager::staticMetaObject,
    qt_meta_stringdata_Reporting__InventoryReportManager.data,
    qt_meta_data_Reporting__InventoryReportManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Reporting::InventoryReportManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Reporting::InventoryReportManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Reporting__InventoryReportManager.stringdata0))
        return static_cast<void*>(this);
    return Reporting::BaseReportManager::qt_metacast(_clname);
}

int Reporting::InventoryReportManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Reporting::BaseReportManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
