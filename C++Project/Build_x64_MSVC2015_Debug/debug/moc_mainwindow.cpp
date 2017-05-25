/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GM-Companion/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[60];
    char stringdata0[1589];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 9), // "playMusic"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 14), // "updateMetaData"
QT_MOC_LITERAL(4, 37, 17), // "updateProgressBar"
QT_MOC_LITERAL(5, 55, 27), // "on_musicPauseButton_clicked"
QT_MOC_LITERAL(6, 83, 26), // "on_musicPlayButton_clicked"
QT_MOC_LITERAL(7, 110, 28), // "on_musicReplayButton_clicked"
QT_MOC_LITERAL(8, 139, 26), // "on_musicNextButton_clicked"
QT_MOC_LITERAL(9, 166, 28), // "on_musicRandomButton_clicked"
QT_MOC_LITERAL(10, 195, 22), // "on_menuMusic_triggered"
QT_MOC_LITERAL(11, 218, 21), // "on_tableDoubleClicked"
QT_MOC_LITERAL(12, 240, 33), // "on_musicVolumeSlider_valueCha..."
QT_MOC_LITERAL(13, 274, 5), // "value"
QT_MOC_LITERAL(14, 280, 9), // "playSound"
QT_MOC_LITERAL(15, 290, 22), // "on_menuSound_triggered"
QT_MOC_LITERAL(16, 313, 26), // "on_soundPlayButton_clicked"
QT_MOC_LITERAL(17, 340, 27), // "on_soundPauseButton_clicked"
QT_MOC_LITERAL(18, 368, 28), // "on_soundReplayButton_clicked"
QT_MOC_LITERAL(19, 397, 26), // "on_soundNextButton_clicked"
QT_MOC_LITERAL(20, 424, 28), // "on_soundRandomButton_clicked"
QT_MOC_LITERAL(21, 453, 33), // "on_soundVolumeSlider_valueCha..."
QT_MOC_LITERAL(22, 487, 6), // "setMap"
QT_MOC_LITERAL(23, 494, 7), // "mapPath"
QT_MOC_LITERAL(24, 502, 21), // "on_menuMaps_triggered"
QT_MOC_LITERAL(25, 524, 27), // "on_mapsZoomInButton_clicked"
QT_MOC_LITERAL(26, 552, 28), // "on_mapsZoomOutButton_clicked"
QT_MOC_LITERAL(27, 581, 30), // "on_mapsFitToViewButton_clicked"
QT_MOC_LITERAL(28, 612, 30), // "on_mapsResetSizeButton_clicked"
QT_MOC_LITERAL(29, 643, 35), // "on_actionSet_Music_Folder_tri..."
QT_MOC_LITERAL(30, 679, 35), // "on_actionSet_Sound_Folder_tri..."
QT_MOC_LITERAL(31, 715, 34), // "on_actionSet_Maps_Folder_trig..."
QT_MOC_LITERAL(32, 750, 39), // "on_actionSet_Resources_Folder..."
QT_MOC_LITERAL(33, 790, 40), // "on_actionSet_Characters_Folde..."
QT_MOC_LITERAL(34, 831, 36), // "on_actionCheck_for_Updates_tr..."
QT_MOC_LITERAL(35, 868, 53), // "on_actionCheck_for_Updates_on..."
QT_MOC_LITERAL(36, 922, 7), // "checked"
QT_MOC_LITERAL(37, 930, 24), // "on_menuGM_Help_triggered"
QT_MOC_LITERAL(38, 955, 23), // "on_actionDice_triggered"
QT_MOC_LITERAL(39, 979, 1), // "t"
QT_MOC_LITERAL(40, 981, 16), // "on_generateNames"
QT_MOC_LITERAL(41, 998, 33), // "on_actionName_Generator_trigg..."
QT_MOC_LITERAL(42, 1032, 23), // "on_characterListClicked"
QT_MOC_LITERAL(43, 1056, 29), // "on_actionCharacters_triggered"
QT_MOC_LITERAL(44, 1086, 32), // "on_createCharacterButton_clicked"
QT_MOC_LITERAL(45, 1119, 33), // "on_updateCharactersButton_cli..."
QT_MOC_LITERAL(46, 1153, 24), // "on_editCharacter_clicked"
QT_MOC_LITERAL(47, 1178, 32), // "on_deleteCharacterButton_clicked"
QT_MOC_LITERAL(48, 1211, 28), // "on_actionOpen_Wiki_triggered"
QT_MOC_LITERAL(49, 1240, 31), // "on_actionReport_a_Bug_triggered"
QT_MOC_LITERAL(50, 1272, 46), // "actionI_want_to_use_an_older_..."
QT_MOC_LITERAL(51, 1319, 38), // "on_versionNetworkAccessManage..."
QT_MOC_LITERAL(52, 1358, 14), // "QNetworkReply*"
QT_MOC_LITERAL(53, 1373, 35), // "on_blogNetworkAccessManagerFi..."
QT_MOC_LITERAL(54, 1409, 23), // "on_radioMetaDataChanged"
QT_MOC_LITERAL(55, 1433, 30), // "on_rivendellPlayButton_clicked"
QT_MOC_LITERAL(56, 1464, 32), // "on_rivendellReloadButton_clicked"
QT_MOC_LITERAL(57, 1497, 33), // "on_actionInternet_Radio_trigg..."
QT_MOC_LITERAL(58, 1531, 27), // "on_mmorpgPlayButton_clicked"
QT_MOC_LITERAL(59, 1559, 29) // "on_mmorpgReloadButton_clicked"

    },
    "MainWindow\0playMusic\0\0updateMetaData\0"
    "updateProgressBar\0on_musicPauseButton_clicked\0"
    "on_musicPlayButton_clicked\0"
    "on_musicReplayButton_clicked\0"
    "on_musicNextButton_clicked\0"
    "on_musicRandomButton_clicked\0"
    "on_menuMusic_triggered\0on_tableDoubleClicked\0"
    "on_musicVolumeSlider_valueChanged\0"
    "value\0playSound\0on_menuSound_triggered\0"
    "on_soundPlayButton_clicked\0"
    "on_soundPauseButton_clicked\0"
    "on_soundReplayButton_clicked\0"
    "on_soundNextButton_clicked\0"
    "on_soundRandomButton_clicked\0"
    "on_soundVolumeSlider_valueChanged\0"
    "setMap\0mapPath\0on_menuMaps_triggered\0"
    "on_mapsZoomInButton_clicked\0"
    "on_mapsZoomOutButton_clicked\0"
    "on_mapsFitToViewButton_clicked\0"
    "on_mapsResetSizeButton_clicked\0"
    "on_actionSet_Music_Folder_triggered\0"
    "on_actionSet_Sound_Folder_triggered\0"
    "on_actionSet_Maps_Folder_triggered\0"
    "on_actionSet_Resources_Folder_triggered\0"
    "on_actionSet_Characters_Folder_triggered\0"
    "on_actionCheck_for_Updates_triggered\0"
    "on_actionCheck_for_Updates_on_Program_Start_triggered\0"
    "checked\0on_menuGM_Help_triggered\0"
    "on_actionDice_triggered\0t\0on_generateNames\0"
    "on_actionName_Generator_triggered\0"
    "on_characterListClicked\0"
    "on_actionCharacters_triggered\0"
    "on_createCharacterButton_clicked\0"
    "on_updateCharactersButton_clicked\0"
    "on_editCharacter_clicked\0"
    "on_deleteCharacterButton_clicked\0"
    "on_actionOpen_Wiki_triggered\0"
    "on_actionReport_a_Bug_triggered\0"
    "actionI_want_to_use_an_older_Version_triggered\0"
    "on_versionNetworkAccessManagerFinished\0"
    "QNetworkReply*\0on_blogNetworkAccessManagerFinished\0"
    "on_radioMetaDataChanged\0"
    "on_rivendellPlayButton_clicked\0"
    "on_rivendellReloadButton_clicked\0"
    "on_actionInternet_Radio_triggered\0"
    "on_mmorpgPlayButton_clicked\0"
    "on_mmorpgReloadButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      53,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  279,    2, 0x08 /* Private */,
       3,    0,  282,    2, 0x08 /* Private */,
       4,    0,  283,    2, 0x08 /* Private */,
       5,    0,  284,    2, 0x08 /* Private */,
       6,    0,  285,    2, 0x08 /* Private */,
       7,    0,  286,    2, 0x08 /* Private */,
       8,    0,  287,    2, 0x08 /* Private */,
       9,    0,  288,    2, 0x08 /* Private */,
      10,    0,  289,    2, 0x08 /* Private */,
      11,    1,  290,    2, 0x08 /* Private */,
      12,    1,  293,    2, 0x08 /* Private */,
      14,    1,  296,    2, 0x08 /* Private */,
      15,    0,  299,    2, 0x08 /* Private */,
      16,    0,  300,    2, 0x08 /* Private */,
      17,    0,  301,    2, 0x08 /* Private */,
      18,    0,  302,    2, 0x08 /* Private */,
      19,    0,  303,    2, 0x08 /* Private */,
      20,    0,  304,    2, 0x08 /* Private */,
      21,    1,  305,    2, 0x08 /* Private */,
      22,    1,  308,    2, 0x08 /* Private */,
      24,    0,  311,    2, 0x08 /* Private */,
      25,    0,  312,    2, 0x08 /* Private */,
      26,    0,  313,    2, 0x08 /* Private */,
      27,    0,  314,    2, 0x08 /* Private */,
      28,    0,  315,    2, 0x08 /* Private */,
      29,    0,  316,    2, 0x08 /* Private */,
      30,    0,  317,    2, 0x08 /* Private */,
      31,    0,  318,    2, 0x08 /* Private */,
      32,    0,  319,    2, 0x08 /* Private */,
      33,    0,  320,    2, 0x08 /* Private */,
      34,    0,  321,    2, 0x08 /* Private */,
      35,    1,  322,    2, 0x08 /* Private */,
      37,    0,  325,    2, 0x08 /* Private */,
      38,    1,  326,    2, 0x08 /* Private */,
      40,    1,  329,    2, 0x08 /* Private */,
      41,    0,  332,    2, 0x08 /* Private */,
      42,    1,  333,    2, 0x08 /* Private */,
      43,    0,  336,    2, 0x08 /* Private */,
      44,    0,  337,    2, 0x08 /* Private */,
      45,    0,  338,    2, 0x08 /* Private */,
      46,    0,  339,    2, 0x08 /* Private */,
      47,    0,  340,    2, 0x08 /* Private */,
      48,    0,  341,    2, 0x08 /* Private */,
      49,    0,  342,    2, 0x08 /* Private */,
      50,    0,  343,    2, 0x08 /* Private */,
      51,    1,  344,    2, 0x08 /* Private */,
      53,    1,  347,    2, 0x08 /* Private */,
      54,    0,  350,    2, 0x08 /* Private */,
      55,    0,  351,    2, 0x08 /* Private */,
      56,    0,  352,    2, 0x08 /* Private */,
      57,    0,  353,    2, 0x08 /* Private */,
      58,    0,  354,    2, 0x08 /* Private */,
      59,    0,  355,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QString,   23,
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
    QMetaType::Void, QMetaType::Bool,   36,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   39,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 52,    2,
    QMetaType::Void, 0x80000000 | 52,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->playMusic((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateMetaData(); break;
        case 2: _t->updateProgressBar(); break;
        case 3: _t->on_musicPauseButton_clicked(); break;
        case 4: _t->on_musicPlayButton_clicked(); break;
        case 5: _t->on_musicReplayButton_clicked(); break;
        case 6: _t->on_musicNextButton_clicked(); break;
        case 7: _t->on_musicRandomButton_clicked(); break;
        case 8: _t->on_menuMusic_triggered(); break;
        case 9: _t->on_tableDoubleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_musicVolumeSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->playSound((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->on_menuSound_triggered(); break;
        case 13: _t->on_soundPlayButton_clicked(); break;
        case 14: _t->on_soundPauseButton_clicked(); break;
        case 15: _t->on_soundReplayButton_clicked(); break;
        case 16: _t->on_soundNextButton_clicked(); break;
        case 17: _t->on_soundRandomButton_clicked(); break;
        case 18: _t->on_soundVolumeSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->setMap((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->on_menuMaps_triggered(); break;
        case 21: _t->on_mapsZoomInButton_clicked(); break;
        case 22: _t->on_mapsZoomOutButton_clicked(); break;
        case 23: _t->on_mapsFitToViewButton_clicked(); break;
        case 24: _t->on_mapsResetSizeButton_clicked(); break;
        case 25: _t->on_actionSet_Music_Folder_triggered(); break;
        case 26: _t->on_actionSet_Sound_Folder_triggered(); break;
        case 27: _t->on_actionSet_Maps_Folder_triggered(); break;
        case 28: _t->on_actionSet_Resources_Folder_triggered(); break;
        case 29: _t->on_actionSet_Characters_Folder_triggered(); break;
        case 30: _t->on_actionCheck_for_Updates_triggered(); break;
        case 31: _t->on_actionCheck_for_Updates_on_Program_Start_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: _t->on_menuGM_Help_triggered(); break;
        case 33: _t->on_actionDice_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->on_generateNames((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 35: _t->on_actionName_Generator_triggered(); break;
        case 36: _t->on_characterListClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->on_actionCharacters_triggered(); break;
        case 38: _t->on_createCharacterButton_clicked(); break;
        case 39: _t->on_updateCharactersButton_clicked(); break;
        case 40: _t->on_editCharacter_clicked(); break;
        case 41: _t->on_deleteCharacterButton_clicked(); break;
        case 42: _t->on_actionOpen_Wiki_triggered(); break;
        case 43: _t->on_actionReport_a_Bug_triggered(); break;
        case 44: _t->actionI_want_to_use_an_older_Version_triggered(); break;
        case 45: _t->on_versionNetworkAccessManagerFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 46: _t->on_blogNetworkAccessManagerFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 47: _t->on_radioMetaDataChanged(); break;
        case 48: _t->on_rivendellPlayButton_clicked(); break;
        case 49: _t->on_rivendellReloadButton_clicked(); break;
        case 50: _t->on_actionInternet_Radio_triggered(); break;
        case 51: _t->on_mmorpgPlayButton_clicked(); break;
        case 52: _t->on_mmorpgReloadButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 45:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 46:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
