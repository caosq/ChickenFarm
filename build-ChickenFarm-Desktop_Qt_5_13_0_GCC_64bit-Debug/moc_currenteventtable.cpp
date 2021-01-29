/****************************************************************************
** Meta object code from reading C++ file 'currenteventtable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Event/currenteventtable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'currenteventtable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CurrentEventTable_t {
    QByteArrayData data[8];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurrentEventTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurrentEventTable_t qt_meta_stringdata_CurrentEventTable = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CurrentEventTable"
QT_MOC_LITERAL(1, 18, 8), // "addEvent"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "sEventItem"
QT_MOC_LITERAL(4, 39, 5), // "mItem"
QT_MOC_LITERAL(5, 45, 8), // "Monitor*"
QT_MOC_LITERAL(6, 54, 8), // "pMonitor"
QT_MOC_LITERAL(7, 63, 14) // "clearTableSlot"

    },
    "CurrentEventTable\0addEvent\0\0sEventItem\0"
    "mItem\0Monitor*\0pMonitor\0clearTableSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurrentEventTable[] = {

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
       1,    2,   24,    2, 0x08 /* Private */,
       7,    0,   29,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void,

       0        // eod
};

void CurrentEventTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CurrentEventTable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addEvent((*reinterpret_cast< sEventItem(*)>(_a[1])),(*reinterpret_cast< Monitor*(*)>(_a[2]))); break;
        case 1: _t->clearTableSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Monitor* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CurrentEventTable::staticMetaObject = { {
    &EventTable::staticMetaObject,
    qt_meta_stringdata_CurrentEventTable.data,
    qt_meta_data_CurrentEventTable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CurrentEventTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurrentEventTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurrentEventTable.stringdata0))
        return static_cast<void*>(this);
    return EventTable::qt_metacast(_clname);
}

int CurrentEventTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EventTable::qt_metacall(_c, _id, _a);
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
