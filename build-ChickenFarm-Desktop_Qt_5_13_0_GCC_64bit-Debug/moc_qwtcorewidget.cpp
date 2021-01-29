/****************************************************************************
** Meta object code from reading C++ file 'qwtcorewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Diagram/qwtcorewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwtcorewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QwtScrollBar_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QwtScrollBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QwtScrollBar_t qt_meta_stringdata_QwtScrollBar = {
    {
QT_MOC_LITERAL(0, 0, 12) // "QwtScrollBar"

    },
    "QwtScrollBar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QwtScrollBar[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QwtScrollBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QwtScrollBar::staticMetaObject = { {
    &QScrollBar::staticMetaObject,
    qt_meta_stringdata_QwtScrollBar.data,
    qt_meta_data_QwtScrollBar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QwtScrollBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QwtScrollBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QwtScrollBar.stringdata0))
        return static_cast<void*>(this);
    return QScrollBar::qt_metacast(_clname);
}

int QwtScrollBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollBar::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_ScrollBarWidget_t {
    QByteArrayData data[9];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScrollBarWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScrollBarWidget_t qt_meta_stringdata_ScrollBarWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ScrollBarWidget"
QT_MOC_LITERAL(1, 16, 10), // "valChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "val"
QT_MOC_LITERAL(4, 32, 7), // "barSLot"
QT_MOC_LITERAL(5, 40, 17), // "sliderReleaseSLot"
QT_MOC_LITERAL(6, 58, 15), // "sliderPressSlot"
QT_MOC_LITERAL(7, 74, 12), // "nextPageSlot"
QT_MOC_LITERAL(8, 87, 15) // "previouPageSLot"

    },
    "ScrollBarWidget\0valChanged\0\0val\0barSLot\0"
    "sliderReleaseSLot\0sliderPressSlot\0"
    "nextPageSlot\0previouPageSLot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScrollBarWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x0a /* Public */,
       5,    0,   50,    2, 0x0a /* Public */,
       6,    0,   51,    2, 0x0a /* Public */,
       7,    0,   52,    2, 0x0a /* Public */,
       8,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ScrollBarWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScrollBarWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->barSLot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sliderReleaseSLot(); break;
        case 3: _t->sliderPressSlot(); break;
        case 4: _t->nextPageSlot(); break;
        case 5: _t->previouPageSLot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ScrollBarWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScrollBarWidget::valChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ScrollBarWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ScrollBarWidget.data,
    qt_meta_data_ScrollBarWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ScrollBarWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScrollBarWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScrollBarWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScrollBarWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void ScrollBarWidget::valChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_QwtCurveCore_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QwtCurveCore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QwtCurveCore_t qt_meta_stringdata_QwtCurveCore = {
    {
QT_MOC_LITERAL(0, 0, 12) // "QwtCurveCore"

    },
    "QwtCurveCore"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QwtCurveCore[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QwtCurveCore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QwtCurveCore::staticMetaObject = { {
    &QwtPlot::staticMetaObject,
    qt_meta_stringdata_QwtCurveCore.data,
    qt_meta_data_QwtCurveCore,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QwtCurveCore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QwtCurveCore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QwtCurveCore.stringdata0))
        return static_cast<void*>(this);
    return QwtPlot::qt_metacast(_clname);
}

int QwtCurveCore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QwtNameItem_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QwtNameItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QwtNameItem_t qt_meta_stringdata_QwtNameItem = {
    {
QT_MOC_LITERAL(0, 0, 11) // "QwtNameItem"

    },
    "QwtNameItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QwtNameItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QwtNameItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QwtNameItem::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QwtNameItem.data,
    qt_meta_data_QwtNameItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QwtNameItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QwtNameItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QwtNameItem.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QwtNameItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QwtCoreWidget_t {
    QByteArrayData data[7];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QwtCoreWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QwtCoreWidget_t qt_meta_stringdata_QwtCoreWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QwtCoreWidget"
QT_MOC_LITERAL(1, 14, 12), // "xScrolChange"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "min"
QT_MOC_LITERAL(4, 32, 3), // "max"
QT_MOC_LITERAL(5, 36, 7), // "BarSlot"
QT_MOC_LITERAL(6, 44, 3) // "val"

    },
    "QwtCoreWidget\0xScrolChange\0\0min\0max\0"
    "BarSlot\0val"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QwtCoreWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void QwtCoreWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QwtCoreWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->xScrolChange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->BarSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QwtCoreWidget::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QwtCoreWidget::xScrolChange)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QwtCoreWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QwtCoreWidget.data,
    qt_meta_data_QwtCoreWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QwtCoreWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QwtCoreWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QwtCoreWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QwtCoreWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QwtCoreWidget::xScrolChange(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
