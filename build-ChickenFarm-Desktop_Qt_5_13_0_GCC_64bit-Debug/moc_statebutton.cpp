/****************************************************************************
** Meta object code from reading C++ file 'statebutton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Button/statebutton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'statebutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_StateButton_t {
    QByteArrayData data[7];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StateButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StateButton_t qt_meta_stringdata_StateButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "StateButton"
QT_MOC_LITERAL(1, 12, 10), // "fontChange"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 8), // "fontSlot"
QT_MOC_LITERAL(4, 33, 8), // "setValue"
QT_MOC_LITERAL(5, 42, 3), // "val"
QT_MOC_LITERAL(6, 46, 11) // "clickedSlot"

    },
    "StateButton\0fontChange\0\0fontSlot\0"
    "setValue\0val\0clickedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StateButton[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x08 /* Private */,
       4,    1,   36,    2, 0x0a /* Public */,
       6,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    5,
    QMetaType::Void,

       0        // eod
};

void StateButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StateButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fontChange(); break;
        case 1: _t->fontSlot(); break;
        case 2: _t->setValue((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 3: _t->clickedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StateButton::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StateButton::fontChange)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject StateButton::staticMetaObject = { {
    &Button::staticMetaObject,
    qt_meta_stringdata_StateButton.data,
    qt_meta_data_StateButton,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StateButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StateButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StateButton.stringdata0))
        return static_cast<void*>(this);
    return Button::qt_metacast(_clname);
}

int StateButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Button::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void StateButton::fontChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
