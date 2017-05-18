/****************************************************************************
** Meta object code from reading C++ file 'charactereditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GM-Companion/charactereditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'charactereditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CharacterEditor_t {
    QByteArrayData data[16];
    char stringdata0[224];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CharacterEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CharacterEditor_t qt_meta_stringdata_CharacterEditor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CharacterEditor"
QT_MOC_LITERAL(1, 16, 13), // "systemChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "save"
QT_MOC_LITERAL(4, 36, 6), // "cancel"
QT_MOC_LITERAL(5, 43, 17), // "skills1AddSkill_u"
QT_MOC_LITERAL(6, 61, 17), // "skills2AddSkill_u"
QT_MOC_LITERAL(7, 79, 17), // "skills3AddSkill_u"
QT_MOC_LITERAL(8, 97, 20), // "skills1RemoveSkill_u"
QT_MOC_LITERAL(9, 118, 20), // "skills2RemoveSkill_u"
QT_MOC_LITERAL(10, 139, 20), // "skills3RemoveSkill_u"
QT_MOC_LITERAL(11, 160, 12), // "inv1AddRow_u"
QT_MOC_LITERAL(12, 173, 12), // "inv2AddRow_u"
QT_MOC_LITERAL(13, 186, 12), // "inv1RemRow_u"
QT_MOC_LITERAL(14, 199, 12), // "inv2RemRow_u"
QT_MOC_LITERAL(15, 212, 11) // "setIconPath"

    },
    "CharacterEditor\0systemChanged\0\0save\0"
    "cancel\0skills1AddSkill_u\0skills2AddSkill_u\0"
    "skills3AddSkill_u\0skills1RemoveSkill_u\0"
    "skills2RemoveSkill_u\0skills3RemoveSkill_u\0"
    "inv1AddRow_u\0inv2AddRow_u\0inv1RemRow_u\0"
    "inv2RemRow_u\0setIconPath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CharacterEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x08 /* Private */,
       3,    0,   87,    2, 0x08 /* Private */,
       4,    0,   88,    2, 0x08 /* Private */,
       5,    0,   89,    2, 0x08 /* Private */,
       6,    0,   90,    2, 0x08 /* Private */,
       7,    0,   91,    2, 0x08 /* Private */,
       8,    0,   92,    2, 0x08 /* Private */,
       9,    0,   93,    2, 0x08 /* Private */,
      10,    0,   94,    2, 0x08 /* Private */,
      11,    0,   95,    2, 0x08 /* Private */,
      12,    0,   96,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,
      14,    0,   98,    2, 0x08 /* Private */,
      15,    0,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CharacterEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CharacterEditor *_t = static_cast<CharacterEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->systemChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->save(); break;
        case 2: _t->cancel(); break;
        case 3: _t->skills1AddSkill_u(); break;
        case 4: _t->skills2AddSkill_u(); break;
        case 5: _t->skills3AddSkill_u(); break;
        case 6: _t->skills1RemoveSkill_u(); break;
        case 7: _t->skills2RemoveSkill_u(); break;
        case 8: _t->skills3RemoveSkill_u(); break;
        case 9: _t->inv1AddRow_u(); break;
        case 10: _t->inv2AddRow_u(); break;
        case 11: _t->inv1RemRow_u(); break;
        case 12: _t->inv2RemRow_u(); break;
        case 13: _t->setIconPath(); break;
        default: ;
        }
    }
}

const QMetaObject CharacterEditor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CharacterEditor.data,
      qt_meta_data_CharacterEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CharacterEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CharacterEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CharacterEditor.stringdata0))
        return static_cast<void*>(const_cast< CharacterEditor*>(this));
    return QObject::qt_metacast(_clname);
}

int CharacterEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
