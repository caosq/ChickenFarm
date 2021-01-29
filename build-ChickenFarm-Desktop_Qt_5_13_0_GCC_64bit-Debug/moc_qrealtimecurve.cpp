/****************************************************************************
** Meta object code from reading C++ file 'qrealtimecurve.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Plot/qrealtimecurve.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qrealtimecurve.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_qrealTimeCurve_t {
    QByteArrayData data[6];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qrealTimeCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qrealTimeCurve_t qt_meta_stringdata_qrealTimeCurve = {
    {
QT_MOC_LITERAL(0, 0, 14), // "qrealTimeCurve"
QT_MOC_LITERAL(1, 15, 15), // "upDateCurveSlot"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 14), // "touchDelaySlot"
QT_MOC_LITERAL(4, 47, 10), // "reloadPlot"
QT_MOC_LITERAL(5, 58, 6) // "rePlot"

    },
    "qrealTimeCurve\0upDateCurveSlot\0\0"
    "touchDelaySlot\0reloadPlot\0rePlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qrealTimeCurve[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void qrealTimeCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<qrealTimeCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->upDateCurveSlot(); break;
        case 1: _t->touchDelaySlot(); break;
        case 2: _t->reloadPlot(); break;
        case 3: _t->rePlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject qrealTimeCurve::staticMetaObject = { {
    &plotWidget::staticMetaObject,
    qt_meta_stringdata_qrealTimeCurve.data,
    qt_meta_data_qrealTimeCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *qrealTimeCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qrealTimeCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_qrealTimeCurve.stringdata0))
        return static_cast<void*>(this);
    return plotWidget::qt_metacast(_clname);
}

int qrealTimeCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = plotWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
