/****************************************************************************
** Meta object code from reading C++ file 'm_calender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Time/m_calender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'm_calender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_m_calender_t {
    QByteArrayData data[10];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_m_calender_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_m_calender_t qt_meta_stringdata_m_calender = {
    {
QT_MOC_LITERAL(0, 0, 10), // "m_calender"
QT_MOC_LITERAL(1, 11, 13), // "PageChangSlot"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "year"
QT_MOC_LITERAL(4, 31, 5), // "month"
QT_MOC_LITERAL(5, 37, 20), // "CurrentDateChangSlot"
QT_MOC_LITERAL(6, 58, 17), // "previousMonthSlot"
QT_MOC_LITERAL(7, 76, 13), // "nextMonthSlot"
QT_MOC_LITERAL(8, 90, 11), // "yearBtnSlot"
QT_MOC_LITERAL(9, 102, 12) // "monthBtnSlot"

    },
    "m_calender\0PageChangSlot\0\0year\0month\0"
    "CurrentDateChangSlot\0previousMonthSlot\0"
    "nextMonthSlot\0yearBtnSlot\0monthBtnSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_m_calender[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x0a /* Public */,
       5,    0,   49,    2, 0x0a /* Public */,
       6,    0,   50,    2, 0x0a /* Public */,
       7,    0,   51,    2, 0x0a /* Public */,
       8,    0,   52,    2, 0x0a /* Public */,
       9,    0,   53,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void m_calender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<m_calender *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PageChangSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->CurrentDateChangSlot(); break;
        case 2: _t->previousMonthSlot(); break;
        case 3: _t->nextMonthSlot(); break;
        case 4: _t->yearBtnSlot(); break;
        case 5: _t->monthBtnSlot(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject m_calender::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_m_calender.data,
    qt_meta_data_m_calender,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *m_calender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *m_calender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_m_calender.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int m_calender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
