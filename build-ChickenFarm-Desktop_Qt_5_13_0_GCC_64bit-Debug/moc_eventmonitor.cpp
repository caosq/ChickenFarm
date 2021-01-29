/****************************************************************************
** Meta object code from reading C++ file 'eventmonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Event/eventmonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventmonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EventMonitor_t {
    QByteArrayData data[10];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EventMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EventMonitor_t qt_meta_stringdata_EventMonitor = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EventMonitor"
QT_MOC_LITERAL(1, 13, 10), // "clearTable"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "eventComing"
QT_MOC_LITERAL(4, 37, 10), // "sEventItem"
QT_MOC_LITERAL(5, 48, 5), // "mItem"
QT_MOC_LITERAL(6, 54, 8), // "Monitor*"
QT_MOC_LITERAL(7, 63, 8), // "pMonitor"
QT_MOC_LITERAL(8, 72, 16), // "eventStringEmpty"
QT_MOC_LITERAL(9, 89, 14) // "valChangedSlot"

    },
    "EventMonitor\0clearTable\0\0eventComing\0"
    "sEventItem\0mItem\0Monitor*\0pMonitor\0"
    "eventStringEmpty\0valChangedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EventMonitor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    2,   35,    2, 0x06 /* Public */,
       8,    2,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void EventMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EventMonitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clearTable(); break;
        case 1: _t->eventComing((*reinterpret_cast< sEventItem(*)>(_a[1])),(*reinterpret_cast< Monitor*(*)>(_a[2]))); break;
        case 2: _t->eventStringEmpty((*reinterpret_cast< sEventItem(*)>(_a[1])),(*reinterpret_cast< Monitor*(*)>(_a[2]))); break;
        case 3: _t->valChangedSlot((*reinterpret_cast< Monitor*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Monitor* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Monitor* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Monitor* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EventMonitor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EventMonitor::clearTable)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EventMonitor::*)(sEventItem , Monitor * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EventMonitor::eventComing)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EventMonitor::*)(sEventItem , Monitor * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EventMonitor::eventStringEmpty)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EventMonitor::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_EventMonitor.data,
    qt_meta_data_EventMonitor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EventMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EventMonitor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int EventMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EventMonitor::clearTable()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void EventMonitor::eventComing(sEventItem _t1, Monitor * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EventMonitor::eventStringEmpty(sEventItem _t1, Monitor * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
