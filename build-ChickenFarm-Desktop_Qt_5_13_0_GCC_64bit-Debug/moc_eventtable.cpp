/****************************************************************************
** Meta object code from reading C++ file 'eventtable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Event/eventtable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventtable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EventTable_t {
    QByteArrayData data[9];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EventTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EventTable_t qt_meta_stringdata_EventTable = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EventTable"
QT_MOC_LITERAL(1, 11, 14), // "langChangeSlot"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "retranslateUi"
QT_MOC_LITERAL(4, 41, 17), // "QLocale::Language"
QT_MOC_LITERAL(5, 59, 4), // "lang"
QT_MOC_LITERAL(6, 64, 13), // "itemPressSlot"
QT_MOC_LITERAL(7, 78, 11), // "QModelIndex"
QT_MOC_LITERAL(8, 90, 5) // "index"

    },
    "EventTable\0langChangeSlot\0\0retranslateUi\0"
    "QLocale::Language\0lang\0itemPressSlot\0"
    "QModelIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EventTable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       6,    1,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void EventTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EventTable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->langChangeSlot(); break;
        case 1: _t->retranslateUi((*reinterpret_cast< QLocale::Language(*)>(_a[1]))); break;
        case 2: _t->itemPressSlot((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EventTable::staticMetaObject = { {
    &QTableWidget::staticMetaObject,
    qt_meta_stringdata_EventTable.data,
    qt_meta_data_EventTable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EventTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EventTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EventTable.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int EventTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
