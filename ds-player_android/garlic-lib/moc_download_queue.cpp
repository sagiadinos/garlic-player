/****************************************************************************
** Meta object code from reading C++ file 'download_queue.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/files/download_queue.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'download_queue.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DownloadQueue_t {
    QByteArrayData data[14];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DownloadQueue_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DownloadQueue_t qt_meta_stringdata_DownloadQueue = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DownloadQueue"
QT_MOC_LITERAL(1, 14, 7), // "succeed"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "src"
QT_MOC_LITERAL(4, 27, 5), // "local"
QT_MOC_LITERAL(5, 33, 11), // "notmodified"
QT_MOC_LITERAL(6, 45, 12), // "notcacheable"
QT_MOC_LITERAL(7, 58, 6), // "failed"
QT_MOC_LITERAL(8, 65, 9), // "doSucceed"
QT_MOC_LITERAL(9, 75, 15), // "TNetworkAccess*"
QT_MOC_LITERAL(10, 91, 10), // "downloader"
QT_MOC_LITERAL(11, 102, 13), // "doNotModified"
QT_MOC_LITERAL(12, 116, 14), // "doNotCacheable"
QT_MOC_LITERAL(13, 131, 8) // "doFailed"

    },
    "DownloadQueue\0succeed\0\0src\0local\0"
    "notmodified\0notcacheable\0failed\0"
    "doSucceed\0TNetworkAccess*\0downloader\0"
    "doNotModified\0doNotCacheable\0doFailed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DownloadQueue[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    1,   59,    2, 0x06 /* Public */,
       6,    1,   62,    2, 0x06 /* Public */,
       7,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   68,    2, 0x0a /* Public */,
      11,    1,   71,    2, 0x0a /* Public */,
      12,    1,   74,    2, 0x0a /* Public */,
      13,    1,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void DownloadQueue::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DownloadQueue *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->succeed((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->notmodified((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->notcacheable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->failed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->doSucceed((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        case 5: _t->doNotModified((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        case 6: _t->doNotCacheable((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        case 7: _t->doFailed((*reinterpret_cast< TNetworkAccess*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TNetworkAccess* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TNetworkAccess* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TNetworkAccess* >(); break;
            }
            break;
        case 7:
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
            using _t = void (DownloadQueue::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownloadQueue::succeed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DownloadQueue::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownloadQueue::notmodified)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DownloadQueue::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownloadQueue::notcacheable)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DownloadQueue::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownloadQueue::failed)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DownloadQueue::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DownloadQueue.data,
    qt_meta_data_DownloadQueue,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DownloadQueue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadQueue::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadQueue.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DownloadQueue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DownloadQueue::succeed(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DownloadQueue::notmodified(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DownloadQueue::notcacheable(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DownloadQueue::failed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
