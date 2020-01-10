/****************************************************************************
** Meta object code from reading C++ file 'file_downloader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/files/file_downloader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'file_downloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FileDownloader_t {
    QByteArrayData data[13];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileDownloader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileDownloader_t qt_meta_stringdata_FileDownloader = {
    {
QT_MOC_LITERAL(0, 0, 14), // "FileDownloader"
QT_MOC_LITERAL(1, 15, 16), // "downloadProgress"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "bytesReceived"
QT_MOC_LITERAL(4, 47, 10), // "bytesTotal"
QT_MOC_LITERAL(5, 58, 18), // "downloadSuccessful"
QT_MOC_LITERAL(6, 77, 13), // "downloadError"
QT_MOC_LITERAL(7, 91, 14), // "QNetworkReply*"
QT_MOC_LITERAL(8, 106, 5), // "reply"
QT_MOC_LITERAL(9, 112, 9), // "goingBusy"
QT_MOC_LITERAL(10, 122, 9), // "backReady"
QT_MOC_LITERAL(11, 132, 8), // "readData"
QT_MOC_LITERAL(12, 141, 14) // "finishDownload"

    },
    "FileDownloader\0downloadProgress\0\0"
    "bytesReceived\0bytesTotal\0downloadSuccessful\0"
    "downloadError\0QNetworkReply*\0reply\0"
    "goingBusy\0backReady\0readData\0"
    "finishDownload"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileDownloader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       5,    0,   54,    2, 0x06 /* Public */,
       6,    1,   55,    2, 0x06 /* Public */,
       9,    0,   58,    2, 0x06 /* Public */,
      10,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   60,    2, 0x09 /* Protected */,
      12,    0,   61,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FileDownloader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileDownloader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 1: _t->downloadSuccessful(); break;
        case 2: _t->downloadError((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 3: _t->goingBusy(); break;
        case 4: _t->backReady(); break;
        case 5: _t->readData(); break;
        case 6: _t->finishDownload(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FileDownloader::*)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileDownloader::downloadProgress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FileDownloader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileDownloader::downloadSuccessful)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FileDownloader::*)(QNetworkReply * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileDownloader::downloadError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FileDownloader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileDownloader::goingBusy)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FileDownloader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileDownloader::backReady)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FileDownloader::staticMetaObject = { {
    &TNetworkAccess::staticMetaObject,
    qt_meta_stringdata_FileDownloader.data,
    qt_meta_data_FileDownloader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FileDownloader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileDownloader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FileDownloader.stringdata0))
        return static_cast<void*>(this);
    return TNetworkAccess::qt_metacast(_clname);
}

int FileDownloader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TNetworkAccess::qt_metacall(_c, _id, _a);
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
void FileDownloader::downloadProgress(qint64 _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileDownloader::downloadSuccessful()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FileDownloader::downloadError(QNetworkReply * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileDownloader::goingBusy()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void FileDownloader::backReady()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
