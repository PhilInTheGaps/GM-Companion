/****************************************************************************
** Meta object code from reading C++ file 'chareditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GM-Companion/chareditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chareditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CharEditor_t {
    QByteArrayData data[8];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CharEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CharEditor_t qt_meta_stringdata_CharEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CharEditor"
QT_MOC_LITERAL(1, 11, 37), // "on_systemComboBox_currentInde..."
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 5), // "index"
QT_MOC_LITERAL(4, 56, 21), // "on_saveButton_clicked"
QT_MOC_LITERAL(5, 78, 23), // "on_cancelButton_clicked"
QT_MOC_LITERAL(6, 102, 37), // "on_addRowSkillsTable1_generic..."
QT_MOC_LITERAL(7, 140, 40) // "on_removeRowSkillsTable1_gene..."

    },
    "CharEditor\0on_systemComboBox_currentIndexChanged\0"
    "\0index\0on_saveButton_clicked\0"
    "on_cancelButton_clicked\0"
    "on_addRowSkillsTable1_generic_clicked\0"
    "on_removeRowSkillsTable1_generic_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CharEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CharEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CharEditor *_t = static_cast<CharEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_systemComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_saveButton_clicked(); break;
        case 2: _t->on_cancelButton_clicked(); break;
        case 3: _t->on_addRowSkillsTable1_generic_clicked(); break;
        case 4: _t->on_removeRowSkillsTable1_generic_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CharEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CharEditor.data,
      qt_meta_data_CharEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CharEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CharEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CharEditor.stringdata0))
        return static_cast<void*>(const_cast< CharEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CharEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
