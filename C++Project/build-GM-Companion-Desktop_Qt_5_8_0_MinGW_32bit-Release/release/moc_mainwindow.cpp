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
    QByteArrayData data[58];
    char stringdata0[1404];
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
QT_MOC_LITERAL(3, 22, 9), // "playSound"
QT_MOC_LITERAL(4, 32, 6), // "setMap"
QT_MOC_LITERAL(5, 39, 7), // "mapPath"
QT_MOC_LITERAL(6, 47, 14), // "updateMetaData"
QT_MOC_LITERAL(7, 62, 17), // "updateProgressBar"
QT_MOC_LITERAL(8, 80, 24), // "on_metaPlayerGotMetadata"
QT_MOC_LITERAL(9, 105, 27), // "on_musicPauseButton_clicked"
QT_MOC_LITERAL(10, 133, 26), // "on_musicPlayButton_clicked"
QT_MOC_LITERAL(11, 160, 28), // "on_musicReplayButton_clicked"
QT_MOC_LITERAL(12, 189, 26), // "on_musicNextButton_clicked"
QT_MOC_LITERAL(13, 216, 25), // "on_setMusicFolder_clicked"
QT_MOC_LITERAL(14, 242, 25), // "on_setSoundFolder_clicked"
QT_MOC_LITERAL(15, 268, 24), // "on_setMapsFolder_clicked"
QT_MOC_LITERAL(16, 293, 29), // "on_setResourcesFolder_clicked"
QT_MOC_LITERAL(17, 323, 24), // "on_menuGM_Help_triggered"
QT_MOC_LITERAL(18, 348, 22), // "on_menuMusic_triggered"
QT_MOC_LITERAL(19, 371, 22), // "on_menuSound_triggered"
QT_MOC_LITERAL(20, 394, 21), // "on_menuMaps_triggered"
QT_MOC_LITERAL(21, 416, 28), // "on_actionMusicPlayer_clicked"
QT_MOC_LITERAL(22, 445, 28), // "on_actionSoundPlayer_clicked"
QT_MOC_LITERAL(23, 474, 27), // "on_actionMapsViewer_clicked"
QT_MOC_LITERAL(24, 502, 21), // "on_actionDice_clicked"
QT_MOC_LITERAL(25, 524, 25), // "on_actionDatabase_clicked"
QT_MOC_LITERAL(26, 550, 19), // "on_openWiki_clicked"
QT_MOC_LITERAL(27, 570, 26), // "on_checkForUpdates_clicked"
QT_MOC_LITERAL(28, 597, 21), // "on_reportABug_clicked"
QT_MOC_LITERAL(29, 619, 34), // "on_iWantToUseAnOlderVersionCl..."
QT_MOC_LITERAL(30, 654, 21), // "on_tableDoubleClicked"
QT_MOC_LITERAL(31, 676, 26), // "on_soundPlayButton_clicked"
QT_MOC_LITERAL(32, 703, 27), // "on_soundPauseButton_clicked"
QT_MOC_LITERAL(33, 731, 28), // "on_soundReplayButton_clicked"
QT_MOC_LITERAL(34, 760, 26), // "on_soundNextButton_clicked"
QT_MOC_LITERAL(35, 787, 33), // "on_musicVolumeSlider_valueCha..."
QT_MOC_LITERAL(36, 821, 5), // "value"
QT_MOC_LITERAL(37, 827, 33), // "on_soundVolumeSlider_valueCha..."
QT_MOC_LITERAL(38, 861, 27), // "on_mapsZoomInButton_clicked"
QT_MOC_LITERAL(39, 889, 28), // "on_mapsZoomOutButton_clicked"
QT_MOC_LITERAL(40, 918, 30), // "on_mapsFitToViewButton_clicked"
QT_MOC_LITERAL(41, 949, 30), // "on_mapsResetSizeButton_clicked"
QT_MOC_LITERAL(42, 980, 31), // "on_networkAccessManagerFinished"
QT_MOC_LITERAL(43, 1012, 14), // "QNetworkReply*"
QT_MOC_LITERAL(44, 1027, 23), // "on_radioMetaDataChanged"
QT_MOC_LITERAL(45, 1051, 30), // "on_rivendellPlayButton_clicked"
QT_MOC_LITERAL(46, 1082, 32), // "on_rivendellReloadButton_clicked"
QT_MOC_LITERAL(47, 1115, 22), // "on_actionRadio_clicked"
QT_MOC_LITERAL(48, 1138, 21), // "on_radioTimer_timeout"
QT_MOC_LITERAL(49, 1160, 37), // "on_radioNetworkAccessManager_..."
QT_MOC_LITERAL(50, 1198, 27), // "on_mmorpgPlayButton_clicked"
QT_MOC_LITERAL(51, 1226, 29), // "on_mmorpgReloadButton_clicked"
QT_MOC_LITERAL(52, 1256, 16), // "on_generateNames"
QT_MOC_LITERAL(53, 1273, 30), // "on_actionNameGenerator_clicked"
QT_MOC_LITERAL(54, 1304, 25), // "on_checkForUpdatesOnStart"
QT_MOC_LITERAL(55, 1330, 23), // "on_characterListClicked"
QT_MOC_LITERAL(56, 1354, 27), // "on_actionCharacters_clicked"
QT_MOC_LITERAL(57, 1382, 21) // "on_pushButton_clicked"

    },
    "MainWindow\0playMusic\0\0playSound\0setMap\0"
    "mapPath\0updateMetaData\0updateProgressBar\0"
    "on_metaPlayerGotMetadata\0"
    "on_musicPauseButton_clicked\0"
    "on_musicPlayButton_clicked\0"
    "on_musicReplayButton_clicked\0"
    "on_musicNextButton_clicked\0"
    "on_setMusicFolder_clicked\0"
    "on_setSoundFolder_clicked\0"
    "on_setMapsFolder_clicked\0"
    "on_setResourcesFolder_clicked\0"
    "on_menuGM_Help_triggered\0"
    "on_menuMusic_triggered\0on_menuSound_triggered\0"
    "on_menuMaps_triggered\0"
    "on_actionMusicPlayer_clicked\0"
    "on_actionSoundPlayer_clicked\0"
    "on_actionMapsViewer_clicked\0"
    "on_actionDice_clicked\0on_actionDatabase_clicked\0"
    "on_openWiki_clicked\0on_checkForUpdates_clicked\0"
    "on_reportABug_clicked\0"
    "on_iWantToUseAnOlderVersionClicked\0"
    "on_tableDoubleClicked\0on_soundPlayButton_clicked\0"
    "on_soundPauseButton_clicked\0"
    "on_soundReplayButton_clicked\0"
    "on_soundNextButton_clicked\0"
    "on_musicVolumeSlider_valueChanged\0"
    "value\0on_soundVolumeSlider_valueChanged\0"
    "on_mapsZoomInButton_clicked\0"
    "on_mapsZoomOutButton_clicked\0"
    "on_mapsFitToViewButton_clicked\0"
    "on_mapsResetSizeButton_clicked\0"
    "on_networkAccessManagerFinished\0"
    "QNetworkReply*\0on_radioMetaDataChanged\0"
    "on_rivendellPlayButton_clicked\0"
    "on_rivendellReloadButton_clicked\0"
    "on_actionRadio_clicked\0on_radioTimer_timeout\0"
    "on_radioNetworkAccessManager_finished\0"
    "on_mmorpgPlayButton_clicked\0"
    "on_mmorpgReloadButton_clicked\0"
    "on_generateNames\0on_actionNameGenerator_clicked\0"
    "on_checkForUpdatesOnStart\0"
    "on_characterListClicked\0"
    "on_actionCharacters_clicked\0"
    "on_pushButton_clicked"
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
       3,    1,  282,    2, 0x08 /* Private */,
       4,    1,  285,    2, 0x08 /* Private */,
       6,    0,  288,    2, 0x08 /* Private */,
       7,    0,  289,    2, 0x08 /* Private */,
       8,    0,  290,    2, 0x08 /* Private */,
       9,    0,  291,    2, 0x08 /* Private */,
      10,    0,  292,    2, 0x08 /* Private */,
      11,    0,  293,    2, 0x08 /* Private */,
      12,    0,  294,    2, 0x08 /* Private */,
      13,    0,  295,    2, 0x08 /* Private */,
      14,    0,  296,    2, 0x08 /* Private */,
      15,    0,  297,    2, 0x08 /* Private */,
      16,    0,  298,    2, 0x08 /* Private */,
      17,    0,  299,    2, 0x08 /* Private */,
      18,    0,  300,    2, 0x08 /* Private */,
      19,    0,  301,    2, 0x08 /* Private */,
      20,    0,  302,    2, 0x08 /* Private */,
      21,    0,  303,    2, 0x08 /* Private */,
      22,    0,  304,    2, 0x08 /* Private */,
      23,    0,  305,    2, 0x08 /* Private */,
      24,    0,  306,    2, 0x08 /* Private */,
      25,    0,  307,    2, 0x08 /* Private */,
      26,    0,  308,    2, 0x08 /* Private */,
      27,    0,  309,    2, 0x08 /* Private */,
      28,    0,  310,    2, 0x08 /* Private */,
      29,    0,  311,    2, 0x08 /* Private */,
      30,    2,  312,    2, 0x08 /* Private */,
      31,    0,  317,    2, 0x08 /* Private */,
      32,    0,  318,    2, 0x08 /* Private */,
      33,    0,  319,    2, 0x08 /* Private */,
      34,    0,  320,    2, 0x08 /* Private */,
      35,    1,  321,    2, 0x08 /* Private */,
      37,    1,  324,    2, 0x08 /* Private */,
      38,    0,  327,    2, 0x08 /* Private */,
      39,    0,  328,    2, 0x08 /* Private */,
      40,    0,  329,    2, 0x08 /* Private */,
      41,    0,  330,    2, 0x08 /* Private */,
      42,    1,  331,    2, 0x08 /* Private */,
      44,    0,  334,    2, 0x08 /* Private */,
      45,    0,  335,    2, 0x08 /* Private */,
      46,    0,  336,    2, 0x08 /* Private */,
      47,    0,  337,    2, 0x08 /* Private */,
      48,    0,  338,    2, 0x08 /* Private */,
      49,    1,  339,    2, 0x08 /* Private */,
      50,    0,  342,    2, 0x08 /* Private */,
      51,    0,  343,    2, 0x08 /* Private */,
      52,    1,  344,    2, 0x08 /* Private */,
      53,    0,  347,    2, 0x08 /* Private */,
      54,    1,  348,    2, 0x08 /* Private */,
      55,    1,  351,    2, 0x08 /* Private */,
      56,    0,  354,    2, 0x08 /* Private */,
      57,    0,  355,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    5,
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
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 43,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 43,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
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
        case 1: _t->playSound((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setMap((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->updateMetaData(); break;
        case 4: _t->updateProgressBar(); break;
        case 5: _t->on_metaPlayerGotMetadata(); break;
        case 6: _t->on_musicPauseButton_clicked(); break;
        case 7: _t->on_musicPlayButton_clicked(); break;
        case 8: _t->on_musicReplayButton_clicked(); break;
        case 9: _t->on_musicNextButton_clicked(); break;
        case 10: _t->on_setMusicFolder_clicked(); break;
        case 11: _t->on_setSoundFolder_clicked(); break;
        case 12: _t->on_setMapsFolder_clicked(); break;
        case 13: _t->on_setResourcesFolder_clicked(); break;
        case 14: _t->on_menuGM_Help_triggered(); break;
        case 15: _t->on_menuMusic_triggered(); break;
        case 16: _t->on_menuSound_triggered(); break;
        case 17: _t->on_menuMaps_triggered(); break;
        case 18: _t->on_actionMusicPlayer_clicked(); break;
        case 19: _t->on_actionSoundPlayer_clicked(); break;
        case 20: _t->on_actionMapsViewer_clicked(); break;
        case 21: _t->on_actionDice_clicked(); break;
        case 22: _t->on_actionDatabase_clicked(); break;
        case 23: _t->on_openWiki_clicked(); break;
        case 24: _t->on_checkForUpdates_clicked(); break;
        case 25: _t->on_reportABug_clicked(); break;
        case 26: _t->on_iWantToUseAnOlderVersionClicked(); break;
        case 27: _t->on_tableDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 28: _t->on_soundPlayButton_clicked(); break;
        case 29: _t->on_soundPauseButton_clicked(); break;
        case 30: _t->on_soundReplayButton_clicked(); break;
        case 31: _t->on_soundNextButton_clicked(); break;
        case 32: _t->on_musicVolumeSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->on_soundVolumeSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->on_mapsZoomInButton_clicked(); break;
        case 35: _t->on_mapsZoomOutButton_clicked(); break;
        case 36: _t->on_mapsFitToViewButton_clicked(); break;
        case 37: _t->on_mapsResetSizeButton_clicked(); break;
        case 38: _t->on_networkAccessManagerFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 39: _t->on_radioMetaDataChanged(); break;
        case 40: _t->on_rivendellPlayButton_clicked(); break;
        case 41: _t->on_rivendellReloadButton_clicked(); break;
        case 42: _t->on_actionRadio_clicked(); break;
        case 43: _t->on_radioTimer_timeout(); break;
        case 44: _t->on_radioNetworkAccessManager_finished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 45: _t->on_mmorpgPlayButton_clicked(); break;
        case 46: _t->on_mmorpgReloadButton_clicked(); break;
        case 47: _t->on_generateNames((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 48: _t->on_actionNameGenerator_clicked(); break;
        case 49: _t->on_checkForUpdatesOnStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 50: _t->on_characterListClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 51: _t->on_actionCharacters_clicked(); break;
        case 52: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 38:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 44:
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
