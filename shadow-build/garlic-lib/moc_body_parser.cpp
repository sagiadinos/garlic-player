/****************************************************************************
** Meta object code from reading C++ file 'body_parser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/garlic-lib/smilparser/body_parser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'body_parser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BodyParser_t {
    QByteArrayData data[20];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BodyParser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BodyParser_t qt_meta_stringdata_BodyParser = {
    {
QT_MOC_LITERAL(0, 0, 10), // "BodyParser"
QT_MOC_LITERAL(1, 11, 14), // "startShowMedia"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "BaseMedia*"
QT_MOC_LITERAL(4, 38, 5), // "media"
QT_MOC_LITERAL(5, 44, 13), // "stopShowMedia"
QT_MOC_LITERAL(6, 58, 12), // "foundElement"
QT_MOC_LITERAL(7, 71, 11), // "TContainer*"
QT_MOC_LITERAL(8, 83, 16), // "parent_container"
QT_MOC_LITERAL(9, 100, 11), // "QDomElement"
QT_MOC_LITERAL(10, 112, 11), // "dom_element"
QT_MOC_LITERAL(11, 124, 14), // "preloadElement"
QT_MOC_LITERAL(12, 139, 12), // "startElement"
QT_MOC_LITERAL(13, 152, 12), // "BaseTimings*"
QT_MOC_LITERAL(14, 165, 7), // "element"
QT_MOC_LITERAL(15, 173, 13), // "finishElement"
QT_MOC_LITERAL(16, 187, 15), // "finishedPreload"
QT_MOC_LITERAL(17, 203, 19), // "resumeQueuedElement"
QT_MOC_LITERAL(18, 223, 19), // "pausePlayingElement"
QT_MOC_LITERAL(19, 243, 18) // "stopPlayingElement"

    },
    "BodyParser\0startShowMedia\0\0BaseMedia*\0"
    "media\0stopShowMedia\0foundElement\0"
    "TContainer*\0parent_container\0QDomElement\0"
    "dom_element\0preloadElement\0startElement\0"
    "BaseTimings*\0element\0finishElement\0"
    "finishedPreload\0resumeQueuedElement\0"
    "pausePlayingElement\0stopPlayingElement"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BodyParser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       5,    1,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   70,    2, 0x09 /* Protected */,
      11,    2,   75,    2, 0x09 /* Protected */,
      12,    2,   80,    2, 0x09 /* Protected */,
      15,    2,   85,    2, 0x09 /* Protected */,
      16,    0,   90,    2, 0x09 /* Protected */,
      17,    1,   91,    2, 0x09 /* Protected */,
      18,    1,   94,    2, 0x09 /* Protected */,
      19,    1,   97,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 9,    8,   10,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 9,    8,   10,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 13,    8,   14,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 13,    8,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void BodyParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BodyParser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 1: _t->stopShowMedia((*reinterpret_cast< BaseMedia*(*)>(_a[1]))); break;
        case 2: _t->foundElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< QDomElement(*)>(_a[2]))); break;
        case 3: _t->preloadElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< QDomElement(*)>(_a[2]))); break;
        case 4: _t->startElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< BaseTimings*(*)>(_a[2]))); break;
        case 5: _t->finishElement((*reinterpret_cast< TContainer*(*)>(_a[1])),(*reinterpret_cast< BaseTimings*(*)>(_a[2]))); break;
        case 6: _t->finishedPreload(); break;
        case 7: _t->resumeQueuedElement((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 8: _t->pausePlayingElement((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        case 9: _t->stopPlayingElement((*reinterpret_cast< BaseTimings*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseMedia* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TContainer* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseTimings* >(); break;
            }
            break;
        case 9:
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
            using _t = void (BodyParser::*)(BaseMedia * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyParser::startShowMedia)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BodyParser::*)(BaseMedia * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BodyParser::stopShowMedia)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BodyParser::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_BodyParser.data,
    qt_meta_data_BodyParser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BodyParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BodyParser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BodyParser.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BodyParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void BodyParser::startShowMedia(BaseMedia * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BodyParser::stopShowMedia(BaseMedia * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
