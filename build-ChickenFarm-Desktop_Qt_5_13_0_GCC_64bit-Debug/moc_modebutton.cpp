/****************************************************************************
** Meta object code from reading C++ file 'modebutton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Widget/Button/modebutton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modebutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_privateMenu_t {
    QByteArrayData data[4];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_privateMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_privateMenu_t qt_meta_stringdata_privateMenu = {
    {
QT_MOC_LITERAL(0, 0, 11), // "privateMenu"
QT_MOC_LITERAL(1, 12, 11), // "valueChange"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10) // "actionSlot"

    },
    "privateMenu\0valueChange\0\0actionSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_privateMenu[] = {

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
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   27,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void privateMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<privateMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->actionSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (privateMenu::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&privateMenu::valueChange)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject privateMenu::staticMetaObject = { {
    &QMenu::staticMetaObject,
    qt_meta_stringdata_privateMenu.data,
    qt_meta_data_privateMenu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *privateMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *privateMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_privateMenu.stringdata0))
        return static_cast<void*>(this);
    return QMenu::qt_metacast(_clname);
}

int privateMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
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
void privateMenu::valueChange(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ModeButton_t {
    QByteArrayData data[8];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModeButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModeButton_t qt_meta_stringdata_ModeButton = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ModeButton"
QT_MOC_LITERAL(1, 11, 10), // "fontChange"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "fontSlot"
QT_MOC_LITERAL(4, 32, 11), // "clickedSlot"
QT_MOC_LITERAL(5, 44, 9), // "valueSlot"
QT_MOC_LITERAL(6, 54, 7), // "tempVal"
QT_MOC_LITERAL(7, 62, 8) // "hideMenu"

    },
    "ModeButton\0fontChange\0\0fontSlot\0"
    "clickedSlot\0valueSlot\0tempVal\0hideMenu"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModeButton[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    1,   42,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

       0        // eod
};

void ModeButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModeButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fontChange(); break;
        case 1: _t->fontSlot(); break;
        case 2: _t->clickedSlot(); break;
        case 3: _t->valueSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->hideMenu(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModeButton::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModeButton::fontChange)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModeButton::staticMetaObject = { {
    &Button::staticMetaObject,
    qt_meta_stringdata_ModeButton.data,
    qt_meta_data_ModeButton,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModeButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModeButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModeButton.stringdata0))
        return static_cast<void*>(this);
    return Button::qt_metacast(_clname);
}

int ModeButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Button::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ModeButton::fontChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
