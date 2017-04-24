/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDice;
    QAction *actionDatabase;
    QAction *actionSet_Music_Folder;
    QAction *actionSet_Sound_Folder;
    QAction *actionSet_Maps_Folder;
    QAction *actionSet_Resources_Folder;
    QAction *actionMusic_Player;
    QAction *actionSound_Player;
    QAction *actionMap_Viewer;
    QAction *actionSet_Database_Path;
    QAction *actionOpen_Wiki;
    QAction *actionCheck_for_Updates;
    QAction *actionReport_a_Bug;
    QAction *actionI_want_to_use_an_older_Version;
    QAction *actionInternet_Radio;
    QAction *actionCheck_for_Updates_on_Program_Start;
    QAction *actionNamne_Generator;
    QAction *actionCharacters;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *infoBox;
    QLabel *musicLabel;
    QHBoxLayout *musicControls;
    QPushButton *musicPlayButton;
    QPushButton *musicPauseButton;
    QPushButton *musicReplayButton;
    QPushButton *musicNextButton;
    QSlider *musicVolumeSlider;
    QFrame *line_2;
    QLabel *soundLabel;
    QHBoxLayout *soundControls;
    QPushButton *soundPlayButton;
    QPushButton *soundPauseButton;
    QPushButton *soundReplayButton;
    QPushButton *soundNextButton;
    QSlider *soundVolumeSlider;
    QFrame *line;
    QLabel *musicInfoLabel;
    QLabel *musicTitleLabel;
    QLabel *musicAlbumLabel;
    QLabel *musicArtistLabel;
    QLabel *musicYearLabel;
    QFrame *line_3;
    QLabel *soundInfoLabel;
    QLabel *soundNameLabel;
    QFrame *line_4;
    QLabel *musicCoverLabel;
    QProgressBar *musicProgressBar;
    QStackedWidget *stackedWidget;
    QWidget *pageGMHelp;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidgetGMHelp;
    QWidget *tabDice;
    QHBoxLayout *horizontalLayout_8;
    QWidget *tabNames;
    QHBoxLayout *horizontalLayout_9;
    QTabWidget *nameTabWidget;
    QTextEdit *nameTextEdit;
    QWidget *tabCharacters;
    QHBoxLayout *horizontalLayout_10;
    QFrame *frame;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_2;
    QListWidget *charactersListWidget;
    QLabel *characterIconLabel;
    QPushButton *createCharacterButton;
    QPushButton *updateCharactersButton;
    QStackedWidget *charactersStackedWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QLabel *label;
    QTextEdit *textEdit;
    QWidget *pageMusic;
    QHBoxLayout *horizontalLayout_5;
    QWidget *pageSound;
    QHBoxLayout *horizontalLayout_3;
    QWidget *pageMaps;
    QHBoxLayout *horizontalLayout_6;
    QFrame *mapsControlFrame;
    QVBoxLayout *verticalLayout_4;
    QPushButton *mapsZoomInButton;
    QPushButton *mapsZoomOutButton;
    QPushButton *mapsFitToViewButton;
    QPushButton *mapsResetSizeButton;
    QFrame *line_5;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *mapsVBox;
    QWidget *pageRadio;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_13;
    QFrame *radioRivendellFrame;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_10;
    QPushButton *rivendellPlayButton;
    QPushButton *rivendellReloadButton;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_7;
    QTextEdit *textEdit_2;
    QSpacerItem *horizontalSpacer;
    QFrame *radioMmorpgDeFrame;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_15;
    QPushButton *mmorpgPlayButton;
    QPushButton *mmorpgReloadButton;
    QVBoxLayout *verticalLayout_16;
    QLabel *label_9;
    QTextEdit *textEdit_3;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuGM_Help;
    QMenu *menuOptions;
    QMenu *menuMusic;
    QMenu *menuSound;
    QMenu *menuMaps;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1300, 769);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/ico/resources/icon256.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QLatin1String("QWidget {\n"
"	background-color: #222222;\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QMenuBar{\n"
"background-color: rgb(102, 102, 102);\n"
"}\n"
"\n"
"QMenuBar::item {\n"
"    spacing: 10px;\n"
"	padding: 4px;\n"
"    background: transparent;\n"
"    border-radius: 2px;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    background: #444444;\n"
"}\n"
"\n"
"QMenuBar::item:hover:!pressed {\n"
"    background: #555555;\n"
"}\n"
"\n"
"QMenuBar::item:pressed {\n"
"    background: #333333;\n"
"}\n"
"\n"
"QMenu {\n"
"    background-color: #666666;\n"
"}\n"
"\n"
"QMenu::item {\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"QMenu::item:selected {\n"
"    background-color: #333333;\n"
"}\n"
"\n"
"QPushButton{\n"
"background-color: rgb(102, 102, 102);\n"
"border: 1px;\n"
"padding: 5px;\n"
"min-width: 40px;\n"
"min-height: 30px;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(51, 51, 51);\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"	background-color: #111111;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed{"
                        "\n"
"background-color: rgb(85, 85, 85);\n"
"}\n"
"\n"
"QSlider::groove:horizontal {\n"
"    border: transparent;\n"
"	height: 10px;\n"
"	background-color: rgb(102, 102, 102);\n"
"    margin: 5px 0;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: white;\n"
"    border: transparent;\n"
"    width: 15px;\n"
"    margin: -3px 0;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QProgressBar {\n"
"    border: 2px solid grey;\n"
"	background-color: rgb(102, 102, 102);\n"
"    border-radius: 0px;\n"
"	text-align: center;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #222222;\n"
"    width: 1px;\n"
"}\n"
"\n"
"QTabWidget::pane { /* The tab widget frame */\n"
"    border: 1px solid #555555;\n"
"}\n"
"\n"
"QTabWidget::tab-bar {\n"
"    left: 0px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"	background-color: rgb(102, 102, 102);\n"
"    border: 1px solid #555555;\n"
"    border-top-left-radius: 2px;\n"
"    border-top-right-radius: 2px;\n"
"    min-width: 100px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QTabBa"
                        "r::tab:selected, QTabBar::tab:hover {\n"
"    background: #555555;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    border-color: #333333;\n"
"    border-bottom-color: #555555; /* same as pane color */\n"
"	background-color: rgb(68, 68, 68);\n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 2px; /* make non-selected tabs look smaller */\n"
"}\n"
"\n"
"QToolBox::tab {\n"
"background-color: rgb(102, 102, 102);\n"
"    border-radius: 1px;\n"
"}\n"
"\n"
"QToolBox::tab:hover:!pressed {\n"
"background-color: rgb(85, 85, 85);\n"
"}\n"
"\n"
"QToolBox::tab:!pressed {\n"
"background-color: rgb(51, 51, 51);\n"
"}\n"
"\n"
"QToolBox::tab:selected { \n"
"    font: italic, bold;\n"
"}\n"
"\n"
"QTableView{\n"
"selection-background-color: rgb(68, 68, 68);\n"
"}\n"
"\n"
"QTableView QTableCornerButton::section {\n"
"background-color: rgb(34, 34, 34);\n"
"}\n"
"\n"
"QTableView::item {\n"
"   /* border: 5px solid rgba(68, 119, 170, 150);*/\n"
"	\n"
"	background-color: #333333;\n"
"}\n"
"\n"
"QTableView::item:selected {\n"
""
                        "	background-color: #777777;\n"
"}\n"
"\n"
"QTableView, QHeaderView::section {    \n"
"	background-color: rgb(34, 34, 34);\n"
"}\n"
"\n"
"QScrollBar:horizontal {\n"
"    border: 2px solid #222222;\n"
"    background-color: #333333;\n"
"    height: 15px;\n"
"    margin: 0px 20px 0 20px;\n"
"}\n"
"QScrollBar::handle:horizontal {\n"
"    background-color: #222222;\n"
"    min-width: 20px;\n"
"}\n"
"QScrollBar::add-line:horizontal {\n"
"    border: 2px solid #444444;\n"
"    background: #222222;\n"
"    width: 20px;\n"
"    subcontrol-position: right;\n"
"    subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:horizontal {\n"
"    border: 2px solid #444444;\n"
"    background: #222222;\n"
"    width: 20px;\n"
"    subcontrol-position: left;\n"
"    subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"    border: 2px solid #222222;\n"
"    background-color: #333333;\n"
"    width: 15px;\n"
"    margin: 20px 0px 20 0px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background-color: #22222"
                        "2;\n"
"    min-height: 20px;\n"
"}\n"
"QScrollBar::add-line:vertical {\n"
"    border: 2px solid #444444;\n"
"    background: #222222;\n"
"    height: 20px;\n"
"    subcontrol-position: bottom;\n"
"    subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:vertical {\n"
"    border: 2px solid #444444;\n"
"    background: #222222;\n"
"    height: 20px;\n"
"    subcontrol-position: top;\n"
"    subcontrol-origin: margin;\n"
"}"));
        MainWindow->setTabShape(QTabWidget::Triangular);
        actionDice = new QAction(MainWindow);
        actionDice->setObjectName(QStringLiteral("actionDice"));
        QFont font;
        actionDice->setFont(font);
        actionDatabase = new QAction(MainWindow);
        actionDatabase->setObjectName(QStringLiteral("actionDatabase"));
        actionDatabase->setEnabled(false);
        actionDatabase->setVisible(false);
        actionSet_Music_Folder = new QAction(MainWindow);
        actionSet_Music_Folder->setObjectName(QStringLiteral("actionSet_Music_Folder"));
        actionSet_Music_Folder->setCheckable(false);
        actionSet_Music_Folder->setChecked(false);
        actionSet_Sound_Folder = new QAction(MainWindow);
        actionSet_Sound_Folder->setObjectName(QStringLiteral("actionSet_Sound_Folder"));
        actionSet_Maps_Folder = new QAction(MainWindow);
        actionSet_Maps_Folder->setObjectName(QStringLiteral("actionSet_Maps_Folder"));
        actionSet_Resources_Folder = new QAction(MainWindow);
        actionSet_Resources_Folder->setObjectName(QStringLiteral("actionSet_Resources_Folder"));
        actionMusic_Player = new QAction(MainWindow);
        actionMusic_Player->setObjectName(QStringLiteral("actionMusic_Player"));
        actionSound_Player = new QAction(MainWindow);
        actionSound_Player->setObjectName(QStringLiteral("actionSound_Player"));
        actionMap_Viewer = new QAction(MainWindow);
        actionMap_Viewer->setObjectName(QStringLiteral("actionMap_Viewer"));
        actionSet_Database_Path = new QAction(MainWindow);
        actionSet_Database_Path->setObjectName(QStringLiteral("actionSet_Database_Path"));
        actionSet_Database_Path->setVisible(false);
        actionOpen_Wiki = new QAction(MainWindow);
        actionOpen_Wiki->setObjectName(QStringLiteral("actionOpen_Wiki"));
        actionCheck_for_Updates = new QAction(MainWindow);
        actionCheck_for_Updates->setObjectName(QStringLiteral("actionCheck_for_Updates"));
        actionReport_a_Bug = new QAction(MainWindow);
        actionReport_a_Bug->setObjectName(QStringLiteral("actionReport_a_Bug"));
        actionI_want_to_use_an_older_Version = new QAction(MainWindow);
        actionI_want_to_use_an_older_Version->setObjectName(QStringLiteral("actionI_want_to_use_an_older_Version"));
        actionInternet_Radio = new QAction(MainWindow);
        actionInternet_Radio->setObjectName(QStringLiteral("actionInternet_Radio"));
        actionCheck_for_Updates_on_Program_Start = new QAction(MainWindow);
        actionCheck_for_Updates_on_Program_Start->setObjectName(QStringLiteral("actionCheck_for_Updates_on_Program_Start"));
        actionCheck_for_Updates_on_Program_Start->setCheckable(true);
        actionCheck_for_Updates_on_Program_Start->setAutoRepeat(true);
        actionNamne_Generator = new QAction(MainWindow);
        actionNamne_Generator->setObjectName(QStringLiteral("actionNamne_Generator"));
        actionCharacters = new QAction(MainWindow);
        actionCharacters->setObjectName(QStringLiteral("actionCharacters"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        infoBox = new QVBoxLayout();
        infoBox->setSpacing(6);
        infoBox->setObjectName(QStringLiteral("infoBox"));
        musicLabel = new QLabel(centralWidget);
        musicLabel->setObjectName(QStringLiteral("musicLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(musicLabel->sizePolicy().hasHeightForWidth());
        musicLabel->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        musicLabel->setFont(font1);

        infoBox->addWidget(musicLabel);

        musicControls = new QHBoxLayout();
        musicControls->setSpacing(6);
        musicControls->setObjectName(QStringLiteral("musicControls"));
        musicControls->setSizeConstraint(QLayout::SetDefaultConstraint);
        musicPlayButton = new QPushButton(centralWidget);
        musicPlayButton->setObjectName(QStringLiteral("musicPlayButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(musicPlayButton->sizePolicy().hasHeightForWidth());
        musicPlayButton->setSizePolicy(sizePolicy2);
        musicPlayButton->setMinimumSize(QSize(50, 40));
        musicPlayButton->setMaximumSize(QSize(68, 45));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        musicPlayButton->setFont(font2);
        musicPlayButton->setAutoDefault(false);
        musicPlayButton->setFlat(false);

        musicControls->addWidget(musicPlayButton);

        musicPauseButton = new QPushButton(centralWidget);
        musicPauseButton->setObjectName(QStringLiteral("musicPauseButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(musicPauseButton->sizePolicy().hasHeightForWidth());
        musicPauseButton->setSizePolicy(sizePolicy3);
        musicPauseButton->setMinimumSize(QSize(50, 40));
        musicPauseButton->setMaximumSize(QSize(68, 45));
        musicPauseButton->setFont(font2);

        musicControls->addWidget(musicPauseButton);

        musicReplayButton = new QPushButton(centralWidget);
        musicReplayButton->setObjectName(QStringLiteral("musicReplayButton"));
        sizePolicy3.setHeightForWidth(musicReplayButton->sizePolicy().hasHeightForWidth());
        musicReplayButton->setSizePolicy(sizePolicy3);
        musicReplayButton->setMinimumSize(QSize(50, 40));
        musicReplayButton->setMaximumSize(QSize(68, 45));
        musicReplayButton->setFont(font2);

        musicControls->addWidget(musicReplayButton);

        musicNextButton = new QPushButton(centralWidget);
        musicNextButton->setObjectName(QStringLiteral("musicNextButton"));
        sizePolicy3.setHeightForWidth(musicNextButton->sizePolicy().hasHeightForWidth());
        musicNextButton->setSizePolicy(sizePolicy3);
        musicNextButton->setMinimumSize(QSize(50, 40));
        musicNextButton->setMaximumSize(QSize(68, 45));
        musicNextButton->setFont(font2);

        musicControls->addWidget(musicNextButton);


        infoBox->addLayout(musicControls);

        musicVolumeSlider = new QSlider(centralWidget);
        musicVolumeSlider->setObjectName(QStringLiteral("musicVolumeSlider"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(musicVolumeSlider->sizePolicy().hasHeightForWidth());
        musicVolumeSlider->setSizePolicy(sizePolicy4);
        musicVolumeSlider->setMinimumSize(QSize(300, 0));
        musicVolumeSlider->setValue(20);
        musicVolumeSlider->setOrientation(Qt::Horizontal);

        infoBox->addWidget(musicVolumeSlider);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line_2);

        soundLabel = new QLabel(centralWidget);
        soundLabel->setObjectName(QStringLiteral("soundLabel"));
        sizePolicy1.setHeightForWidth(soundLabel->sizePolicy().hasHeightForWidth());
        soundLabel->setSizePolicy(sizePolicy1);
        soundLabel->setFont(font1);

        infoBox->addWidget(soundLabel);

        soundControls = new QHBoxLayout();
        soundControls->setSpacing(6);
        soundControls->setObjectName(QStringLiteral("soundControls"));
        soundPlayButton = new QPushButton(centralWidget);
        soundPlayButton->setObjectName(QStringLiteral("soundPlayButton"));
        sizePolicy3.setHeightForWidth(soundPlayButton->sizePolicy().hasHeightForWidth());
        soundPlayButton->setSizePolicy(sizePolicy3);
        soundPlayButton->setMinimumSize(QSize(50, 40));
        soundPlayButton->setMaximumSize(QSize(68, 45));
        soundPlayButton->setFont(font2);

        soundControls->addWidget(soundPlayButton);

        soundPauseButton = new QPushButton(centralWidget);
        soundPauseButton->setObjectName(QStringLiteral("soundPauseButton"));
        sizePolicy3.setHeightForWidth(soundPauseButton->sizePolicy().hasHeightForWidth());
        soundPauseButton->setSizePolicy(sizePolicy3);
        soundPauseButton->setMinimumSize(QSize(50, 40));
        soundPauseButton->setMaximumSize(QSize(68, 45));
        soundPauseButton->setFont(font2);

        soundControls->addWidget(soundPauseButton);

        soundReplayButton = new QPushButton(centralWidget);
        soundReplayButton->setObjectName(QStringLiteral("soundReplayButton"));
        sizePolicy3.setHeightForWidth(soundReplayButton->sizePolicy().hasHeightForWidth());
        soundReplayButton->setSizePolicy(sizePolicy3);
        soundReplayButton->setMinimumSize(QSize(50, 40));
        soundReplayButton->setMaximumSize(QSize(68, 45));
        soundReplayButton->setFont(font2);

        soundControls->addWidget(soundReplayButton);

        soundNextButton = new QPushButton(centralWidget);
        soundNextButton->setObjectName(QStringLiteral("soundNextButton"));
        sizePolicy3.setHeightForWidth(soundNextButton->sizePolicy().hasHeightForWidth());
        soundNextButton->setSizePolicy(sizePolicy3);
        soundNextButton->setMinimumSize(QSize(50, 40));
        soundNextButton->setMaximumSize(QSize(68, 45));
        soundNextButton->setFont(font2);

        soundControls->addWidget(soundNextButton);


        infoBox->addLayout(soundControls);

        soundVolumeSlider = new QSlider(centralWidget);
        soundVolumeSlider->setObjectName(QStringLiteral("soundVolumeSlider"));
        sizePolicy4.setHeightForWidth(soundVolumeSlider->sizePolicy().hasHeightForWidth());
        soundVolumeSlider->setSizePolicy(sizePolicy4);
        soundVolumeSlider->setMinimumSize(QSize(300, 0));
        soundVolumeSlider->setValue(20);
        soundVolumeSlider->setOrientation(Qt::Horizontal);

        infoBox->addWidget(soundVolumeSlider);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy5);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line);

        musicInfoLabel = new QLabel(centralWidget);
        musicInfoLabel->setObjectName(QStringLiteral("musicInfoLabel"));
        sizePolicy1.setHeightForWidth(musicInfoLabel->sizePolicy().hasHeightForWidth());
        musicInfoLabel->setSizePolicy(sizePolicy1);
        musicInfoLabel->setFont(font1);

        infoBox->addWidget(musicInfoLabel);

        musicTitleLabel = new QLabel(centralWidget);
        musicTitleLabel->setObjectName(QStringLiteral("musicTitleLabel"));
        sizePolicy1.setHeightForWidth(musicTitleLabel->sizePolicy().hasHeightForWidth());
        musicTitleLabel->setSizePolicy(sizePolicy1);
        musicTitleLabel->setMaximumSize(QSize(300, 16777215));
        QFont font3;
        font3.setPointSize(11);
        musicTitleLabel->setFont(font3);
        musicTitleLabel->setWordWrap(true);

        infoBox->addWidget(musicTitleLabel);

        musicAlbumLabel = new QLabel(centralWidget);
        musicAlbumLabel->setObjectName(QStringLiteral("musicAlbumLabel"));
        sizePolicy1.setHeightForWidth(musicAlbumLabel->sizePolicy().hasHeightForWidth());
        musicAlbumLabel->setSizePolicy(sizePolicy1);
        musicAlbumLabel->setMaximumSize(QSize(300, 16777215));
        musicAlbumLabel->setFont(font3);
        musicAlbumLabel->setWordWrap(true);

        infoBox->addWidget(musicAlbumLabel);

        musicArtistLabel = new QLabel(centralWidget);
        musicArtistLabel->setObjectName(QStringLiteral("musicArtistLabel"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(musicArtistLabel->sizePolicy().hasHeightForWidth());
        musicArtistLabel->setSizePolicy(sizePolicy6);
        musicArtistLabel->setMaximumSize(QSize(300, 16777215));
        musicArtistLabel->setFont(font3);
        musicArtistLabel->setWordWrap(true);

        infoBox->addWidget(musicArtistLabel);

        musicYearLabel = new QLabel(centralWidget);
        musicYearLabel->setObjectName(QStringLiteral("musicYearLabel"));
        sizePolicy6.setHeightForWidth(musicYearLabel->sizePolicy().hasHeightForWidth());
        musicYearLabel->setSizePolicy(sizePolicy6);
        musicYearLabel->setMaximumSize(QSize(300, 16777215));
        musicYearLabel->setFont(font3);
        musicYearLabel->setWordWrap(true);

        infoBox->addWidget(musicYearLabel);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        sizePolicy5.setHeightForWidth(line_3->sizePolicy().hasHeightForWidth());
        line_3->setSizePolicy(sizePolicy5);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line_3);

        soundInfoLabel = new QLabel(centralWidget);
        soundInfoLabel->setObjectName(QStringLiteral("soundInfoLabel"));
        sizePolicy6.setHeightForWidth(soundInfoLabel->sizePolicy().hasHeightForWidth());
        soundInfoLabel->setSizePolicy(sizePolicy6);
        soundInfoLabel->setFont(font1);

        infoBox->addWidget(soundInfoLabel);

        soundNameLabel = new QLabel(centralWidget);
        soundNameLabel->setObjectName(QStringLiteral("soundNameLabel"));
        sizePolicy6.setHeightForWidth(soundNameLabel->sizePolicy().hasHeightForWidth());
        soundNameLabel->setSizePolicy(sizePolicy6);
        soundNameLabel->setMaximumSize(QSize(300, 16777215));
        soundNameLabel->setFont(font3);
        soundNameLabel->setWordWrap(true);

        infoBox->addWidget(soundNameLabel);

        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        sizePolicy5.setHeightForWidth(line_4->sizePolicy().hasHeightForWidth());
        line_4->setSizePolicy(sizePolicy5);
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line_4);

        musicCoverLabel = new QLabel(centralWidget);
        musicCoverLabel->setObjectName(QStringLiteral("musicCoverLabel"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(musicCoverLabel->sizePolicy().hasHeightForWidth());
        musicCoverLabel->setSizePolicy(sizePolicy7);
        musicCoverLabel->setMinimumSize(QSize(300, 300));
        musicCoverLabel->setFrameShape(QFrame::NoFrame);
        musicCoverLabel->setFrameShadow(QFrame::Plain);

        infoBox->addWidget(musicCoverLabel);

        musicProgressBar = new QProgressBar(centralWidget);
        musicProgressBar->setObjectName(QStringLiteral("musicProgressBar"));
        sizePolicy4.setHeightForWidth(musicProgressBar->sizePolicy().hasHeightForWidth());
        musicProgressBar->setSizePolicy(sizePolicy4);
        musicProgressBar->setMinimumSize(QSize(300, 0));
        musicProgressBar->setMaximumSize(QSize(300, 16777215));
        musicProgressBar->setAutoFillBackground(false);
        musicProgressBar->setStyleSheet(QStringLiteral(""));
        musicProgressBar->setMaximum(1);
        musicProgressBar->setValue(0);
        musicProgressBar->setTextVisible(true);
        musicProgressBar->setTextDirection(QProgressBar::TopToBottom);

        infoBox->addWidget(musicProgressBar);


        horizontalLayout_2->addLayout(infoBox);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        pageGMHelp = new QWidget();
        pageGMHelp->setObjectName(QStringLiteral("pageGMHelp"));
        verticalLayout_2 = new QVBoxLayout(pageGMHelp);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidgetGMHelp = new QTabWidget(pageGMHelp);
        tabWidgetGMHelp->setObjectName(QStringLiteral("tabWidgetGMHelp"));
        tabDice = new QWidget();
        tabDice->setObjectName(QStringLiteral("tabDice"));
        horizontalLayout_8 = new QHBoxLayout(tabDice);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        tabWidgetGMHelp->addTab(tabDice, QString());
        tabNames = new QWidget();
        tabNames->setObjectName(QStringLiteral("tabNames"));
        horizontalLayout_9 = new QHBoxLayout(tabNames);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        nameTabWidget = new QTabWidget(tabNames);
        nameTabWidget->setObjectName(QStringLiteral("nameTabWidget"));
        QSizePolicy sizePolicy8(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(nameTabWidget->sizePolicy().hasHeightForWidth());
        nameTabWidget->setSizePolicy(sizePolicy8);

        horizontalLayout_9->addWidget(nameTabWidget);

        nameTextEdit = new QTextEdit(tabNames);
        nameTextEdit->setObjectName(QStringLiteral("nameTextEdit"));
        QFont font4;
        font4.setPointSize(20);
        nameTextEdit->setFont(font4);
        nameTextEdit->setOverwriteMode(false);

        horizontalLayout_9->addWidget(nameTextEdit);

        tabWidgetGMHelp->addTab(tabNames, QString());
        tabCharacters = new QWidget();
        tabCharacters->setObjectName(QStringLiteral("tabCharacters"));
        horizontalLayout_10 = new QHBoxLayout(tabCharacters);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        frame = new QFrame(tabCharacters);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(200, 0));
        frame->setMaximumSize(QSize(200, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_5->addWidget(label_2);

        charactersListWidget = new QListWidget(frame);
        charactersListWidget->setObjectName(QStringLiteral("charactersListWidget"));
        charactersListWidget->setMinimumSize(QSize(150, 0));
        charactersListWidget->setMaximumSize(QSize(200, 16777215));
        charactersListWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        charactersListWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
        charactersListWidget->setDefaultDropAction(Qt::IgnoreAction);
        charactersListWidget->setTextElideMode(Qt::ElideLeft);
        charactersListWidget->setMovement(QListView::Static);
        charactersListWidget->setProperty("isWrapping", QVariant(false));
        charactersListWidget->setViewMode(QListView::ListMode);
        charactersListWidget->setUniformItemSizes(false);
        charactersListWidget->setWordWrap(false);
        charactersListWidget->setSelectionRectVisible(true);
        charactersListWidget->setSortingEnabled(false);

        verticalLayout_5->addWidget(charactersListWidget);

        characterIconLabel = new QLabel(frame);
        characterIconLabel->setObjectName(QStringLiteral("characterIconLabel"));
        sizePolicy.setHeightForWidth(characterIconLabel->sizePolicy().hasHeightForWidth());
        characterIconLabel->setSizePolicy(sizePolicy);
        characterIconLabel->setMinimumSize(QSize(0, 0));
        characterIconLabel->setMaximumSize(QSize(180, 180));
        characterIconLabel->setFrameShape(QFrame::Box);
        characterIconLabel->setFrameShadow(QFrame::Raised);

        verticalLayout_5->addWidget(characterIconLabel);

        createCharacterButton = new QPushButton(frame);
        createCharacterButton->setObjectName(QStringLiteral("createCharacterButton"));
        createCharacterButton->setCheckable(false);
        createCharacterButton->setChecked(false);

        verticalLayout_5->addWidget(createCharacterButton);

        updateCharactersButton = new QPushButton(frame);
        updateCharactersButton->setObjectName(QStringLiteral("updateCharactersButton"));

        verticalLayout_5->addWidget(updateCharactersButton);


        horizontalLayout_10->addWidget(frame);

        charactersStackedWidget = new QStackedWidget(tabCharacters);
        charactersStackedWidget->setObjectName(QStringLiteral("charactersStackedWidget"));
        sizePolicy.setHeightForWidth(charactersStackedWidget->sizePolicy().hasHeightForWidth());
        charactersStackedWidget->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(charactersStackedWidget);

        tabWidgetGMHelp->addTab(tabCharacters, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label = new QLabel(tab_3);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_6->addWidget(label);

        textEdit = new QTextEdit(tab_3);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout_6->addWidget(textEdit);

        tabWidgetGMHelp->addTab(tab_3, QString());

        verticalLayout_2->addWidget(tabWidgetGMHelp);

        stackedWidget->addWidget(pageGMHelp);
        pageMusic = new QWidget();
        pageMusic->setObjectName(QStringLiteral("pageMusic"));
        horizontalLayout_5 = new QHBoxLayout(pageMusic);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        stackedWidget->addWidget(pageMusic);
        pageSound = new QWidget();
        pageSound->setObjectName(QStringLiteral("pageSound"));
        horizontalLayout_3 = new QHBoxLayout(pageSound);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        stackedWidget->addWidget(pageSound);
        pageMaps = new QWidget();
        pageMaps->setObjectName(QStringLiteral("pageMaps"));
        horizontalLayout_6 = new QHBoxLayout(pageMaps);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        mapsControlFrame = new QFrame(pageMaps);
        mapsControlFrame->setObjectName(QStringLiteral("mapsControlFrame"));
        sizePolicy8.setHeightForWidth(mapsControlFrame->sizePolicy().hasHeightForWidth());
        mapsControlFrame->setSizePolicy(sizePolicy8);
        mapsControlFrame->setFrameShape(QFrame::StyledPanel);
        mapsControlFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(mapsControlFrame);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        mapsZoomInButton = new QPushButton(mapsControlFrame);
        mapsZoomInButton->setObjectName(QStringLiteral("mapsZoomInButton"));
        mapsZoomInButton->setMinimumSize(QSize(50, 40));
        QFont font5;
        font5.setBold(true);
        font5.setWeight(75);
        mapsZoomInButton->setFont(font5);

        verticalLayout_4->addWidget(mapsZoomInButton);

        mapsZoomOutButton = new QPushButton(mapsControlFrame);
        mapsZoomOutButton->setObjectName(QStringLiteral("mapsZoomOutButton"));
        mapsZoomOutButton->setMinimumSize(QSize(50, 40));
        mapsZoomOutButton->setFont(font5);

        verticalLayout_4->addWidget(mapsZoomOutButton);

        mapsFitToViewButton = new QPushButton(mapsControlFrame);
        mapsFitToViewButton->setObjectName(QStringLiteral("mapsFitToViewButton"));
        mapsFitToViewButton->setMinimumSize(QSize(50, 40));
        mapsFitToViewButton->setFont(font5);

        verticalLayout_4->addWidget(mapsFitToViewButton);

        mapsResetSizeButton = new QPushButton(mapsControlFrame);
        mapsResetSizeButton->setObjectName(QStringLiteral("mapsResetSizeButton"));
        mapsResetSizeButton->setMinimumSize(QSize(50, 40));
        mapsResetSizeButton->setFont(font5);

        verticalLayout_4->addWidget(mapsResetSizeButton);

        line_5 = new QFrame(mapsControlFrame);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        horizontalLayout_6->addWidget(mapsControlFrame);

        mapsVBox = new QVBoxLayout();
        mapsVBox->setSpacing(6);
        mapsVBox->setObjectName(QStringLiteral("mapsVBox"));

        horizontalLayout_6->addLayout(mapsVBox);

        stackedWidget->addWidget(pageMaps);
        pageRadio = new QWidget();
        pageRadio->setObjectName(QStringLiteral("pageRadio"));
        verticalLayout_9 = new QVBoxLayout(pageRadio);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        label_5 = new QLabel(pageRadio);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);
        QFont font6;
        font6.setPointSize(30);
        label_5->setFont(font6);

        verticalLayout_9->addWidget(label_5);

        scrollArea = new QScrollArea(pageRadio);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 968, 628));
        verticalLayout_13 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        radioRivendellFrame = new QFrame(scrollAreaWidgetContents);
        radioRivendellFrame->setObjectName(QStringLiteral("radioRivendellFrame"));
        sizePolicy1.setHeightForWidth(radioRivendellFrame->sizePolicy().hasHeightForWidth());
        radioRivendellFrame->setSizePolicy(sizePolicy1);
        radioRivendellFrame->setMaximumSize(QSize(1000, 450));
        radioRivendellFrame->setFrameShape(QFrame::Box);
        radioRivendellFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_12 = new QVBoxLayout(radioRivendellFrame);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        label_6 = new QLabel(radioRivendellFrame);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font4);

        verticalLayout_12->addWidget(label_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        rivendellPlayButton = new QPushButton(radioRivendellFrame);
        rivendellPlayButton->setObjectName(QStringLiteral("rivendellPlayButton"));
        QSizePolicy sizePolicy9(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(rivendellPlayButton->sizePolicy().hasHeightForWidth());
        rivendellPlayButton->setSizePolicy(sizePolicy9);
        rivendellPlayButton->setMinimumSize(QSize(90, 40));
        rivendellPlayButton->setMaximumSize(QSize(100, 16777215));
        QFont font7;
        font7.setPointSize(15);
        font7.setBold(false);
        font7.setWeight(50);
        rivendellPlayButton->setFont(font7);
        rivendellPlayButton->setStyleSheet(QLatin1String("QPushButton{\n"
"background-color: rgb(102, 102, 102);\n"
"border: 1px;\n"
"padding: 5px;\n"
"min-width: 80px;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(51, 51, 51);\n"
"}\n"
"\n"
"QPushButton:hover:!pressed{\n"
"background-color: rgb(85, 85, 85);\n"
"}"));

        verticalLayout_10->addWidget(rivendellPlayButton);

        rivendellReloadButton = new QPushButton(radioRivendellFrame);
        rivendellReloadButton->setObjectName(QStringLiteral("rivendellReloadButton"));
        sizePolicy9.setHeightForWidth(rivendellReloadButton->sizePolicy().hasHeightForWidth());
        rivendellReloadButton->setSizePolicy(sizePolicy9);
        rivendellReloadButton->setMinimumSize(QSize(90, 40));
        rivendellReloadButton->setMaximumSize(QSize(100, 16777215));
        rivendellReloadButton->setFont(font7);
        rivendellReloadButton->setStyleSheet(QLatin1String("QPushButton{\n"
"background-color: rgb(102, 102, 102);\n"
"border: 1px;\n"
"padding: 5px;\n"
"min-width: 80px;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(51, 51, 51);\n"
"}\n"
"\n"
"QPushButton:hover:!pressed{\n"
"background-color: rgb(85, 85, 85);\n"
"}"));

        verticalLayout_10->addWidget(rivendellReloadButton);


        horizontalLayout_4->addLayout(verticalLayout_10);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        label_7 = new QLabel(radioRivendellFrame);
        label_7->setObjectName(QStringLiteral("label_7"));
        QSizePolicy sizePolicy10(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy10);
        label_7->setMinimumSize(QSize(300, 0));
        label_7->setMaximumSize(QSize(820, 2000));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/radio/RadioRivendell/resources/radio/logo-radio-rivendell.png")));
        label_7->setScaledContents(true);

        verticalLayout_11->addWidget(label_7);

        textEdit_2 = new QTextEdit(radioRivendellFrame);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        QSizePolicy sizePolicy11(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(textEdit_2->sizePolicy().hasHeightForWidth());
        textEdit_2->setSizePolicy(sizePolicy11);
        textEdit_2->setMaximumSize(QSize(820, 16777215));
        textEdit_2->setTabChangesFocus(false);
        textEdit_2->setReadOnly(true);
        textEdit_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout_11->addWidget(textEdit_2);


        horizontalLayout_4->addLayout(verticalLayout_11);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_12->addLayout(horizontalLayout_4);


        verticalLayout_13->addWidget(radioRivendellFrame);

        radioMmorpgDeFrame = new QFrame(scrollAreaWidgetContents);
        radioMmorpgDeFrame->setObjectName(QStringLiteral("radioMmorpgDeFrame"));
        sizePolicy1.setHeightForWidth(radioMmorpgDeFrame->sizePolicy().hasHeightForWidth());
        radioMmorpgDeFrame->setSizePolicy(sizePolicy1);
        radioMmorpgDeFrame->setMaximumSize(QSize(1000, 450));
        radioMmorpgDeFrame->setFrameShape(QFrame::Box);
        radioMmorpgDeFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_14 = new QVBoxLayout(radioMmorpgDeFrame);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        label_8 = new QLabel(radioMmorpgDeFrame);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font4);

        verticalLayout_14->addWidget(label_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        mmorpgPlayButton = new QPushButton(radioMmorpgDeFrame);
        mmorpgPlayButton->setObjectName(QStringLiteral("mmorpgPlayButton"));
        sizePolicy9.setHeightForWidth(mmorpgPlayButton->sizePolicy().hasHeightForWidth());
        mmorpgPlayButton->setSizePolicy(sizePolicy9);
        mmorpgPlayButton->setMinimumSize(QSize(90, 40));
        mmorpgPlayButton->setMaximumSize(QSize(100, 16777215));
        QFont font8;
        font8.setPointSize(15);
        mmorpgPlayButton->setFont(font8);
        mmorpgPlayButton->setStyleSheet(QLatin1String("QPushButton{\n"
"background-color: rgb(102, 102, 102);\n"
"border: 1px;\n"
"padding: 5px;\n"
"min-width: 80px;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(51, 51, 51);\n"
"}\n"
"\n"
"QPushButton:hover:!pressed{\n"
"background-color: rgb(85, 85, 85);\n"
"}"));

        verticalLayout_15->addWidget(mmorpgPlayButton);

        mmorpgReloadButton = new QPushButton(radioMmorpgDeFrame);
        mmorpgReloadButton->setObjectName(QStringLiteral("mmorpgReloadButton"));
        sizePolicy9.setHeightForWidth(mmorpgReloadButton->sizePolicy().hasHeightForWidth());
        mmorpgReloadButton->setSizePolicy(sizePolicy9);
        mmorpgReloadButton->setMinimumSize(QSize(90, 40));
        mmorpgReloadButton->setMaximumSize(QSize(100, 16777215));
        mmorpgReloadButton->setFont(font8);
        mmorpgReloadButton->setStyleSheet(QLatin1String("QPushButton{\n"
"background-color: rgb(102, 102, 102);\n"
"border: 1px;\n"
"padding: 5px;\n"
"min-width: 80px;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(51, 51, 51);\n"
"}\n"
"\n"
"QPushButton:hover:!pressed{\n"
"background-color: rgb(85, 85, 85);\n"
"}"));

        verticalLayout_15->addWidget(mmorpgReloadButton);


        horizontalLayout_7->addLayout(verticalLayout_15);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        label_9 = new QLabel(radioMmorpgDeFrame);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy10.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy10);
        label_9->setMaximumSize(QSize(820, 16777215));
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/radio/RadioMMORPG_de/resources/radio/logo-mmorp-radio-de.png")));
        label_9->setScaledContents(true);

        verticalLayout_16->addWidget(label_9);

        textEdit_3 = new QTextEdit(radioMmorpgDeFrame);
        textEdit_3->setObjectName(QStringLiteral("textEdit_3"));
        sizePolicy11.setHeightForWidth(textEdit_3->sizePolicy().hasHeightForWidth());
        textEdit_3->setSizePolicy(sizePolicy11);
        textEdit_3->setMaximumSize(QSize(820, 16777215));
        textEdit_3->setReadOnly(true);
        textEdit_3->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout_16->addWidget(textEdit_3);


        horizontalLayout_7->addLayout(verticalLayout_16);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);


        verticalLayout_14->addLayout(horizontalLayout_7);


        verticalLayout_13->addWidget(radioMmorpgDeFrame);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_9->addWidget(scrollArea);

        stackedWidget->addWidget(pageRadio);

        horizontalLayout_2->addWidget(stackedWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1300, 28));
        QSizePolicy sizePolicy12(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy12.setHorizontalStretch(0);
        sizePolicy12.setVerticalStretch(0);
        sizePolicy12.setHeightForWidth(menuBar->sizePolicy().hasHeightForWidth());
        menuBar->setSizePolicy(sizePolicy12);
        menuBar->setMinimumSize(QSize(0, 25));
        menuBar->setBaseSize(QSize(0, 25));
        menuBar->setFont(font1);
        menuBar->setStyleSheet(QStringLiteral(""));
        menuGM_Help = new QMenu(menuBar);
        menuGM_Help->setObjectName(QStringLiteral("menuGM_Help"));
        QFont font9;
        font9.setPointSize(10);
        font9.setBold(false);
        font9.setWeight(50);
        menuGM_Help->setFont(font9);
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuOptions->setFont(font9);
        menuMusic = new QMenu(menuBar);
        menuMusic->setObjectName(QStringLiteral("menuMusic"));
        menuMusic->setFont(font9);
        menuSound = new QMenu(menuBar);
        menuSound->setObjectName(QStringLiteral("menuSound"));
        menuSound->setFont(font9);
        menuMaps = new QMenu(menuBar);
        menuMaps->setObjectName(QStringLiteral("menuMaps"));
        menuMaps->setFont(font9);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        QFont font10;
        font10.setPointSize(10);
        menuHelp->setFont(font10);
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGM_Help->menuAction());
        menuBar->addAction(menuMusic->menuAction());
        menuBar->addAction(menuSound->menuAction());
        menuBar->addAction(menuMaps->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuGM_Help->addAction(actionDice);
        menuGM_Help->addAction(actionDatabase);
        menuGM_Help->addAction(actionNamne_Generator);
        menuGM_Help->addAction(actionCharacters);
        menuGM_Help->addSeparator();
        menuGM_Help->addAction(actionSet_Database_Path);
        menuOptions->addAction(actionSet_Resources_Folder);
        menuOptions->addSeparator();
        menuOptions->addAction(actionCheck_for_Updates);
        menuOptions->addAction(actionCheck_for_Updates_on_Program_Start);
        menuMusic->addAction(actionMusic_Player);
        menuMusic->addAction(actionInternet_Radio);
        menuMusic->addSeparator();
        menuMusic->addAction(actionSet_Music_Folder);
        menuSound->addAction(actionSound_Player);
        menuSound->addSeparator();
        menuSound->addAction(actionSet_Sound_Folder);
        menuMaps->addAction(actionMap_Viewer);
        menuMaps->addSeparator();
        menuMaps->addAction(actionSet_Maps_Folder);
        menuHelp->addAction(actionOpen_Wiki);
        menuHelp->addAction(actionReport_a_Bug);
        menuHelp->addSeparator();
        menuHelp->addAction(actionI_want_to_use_an_older_Version);

        retranslateUi(MainWindow);

        musicPlayButton->setDefault(false);
        stackedWidget->setCurrentIndex(0);
        tabWidgetGMHelp->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GM-Companion | DEV-BUILD Beta 3.0 PRE 05", Q_NULLPTR));
        actionDice->setText(QApplication::translate("MainWindow", "Dice", Q_NULLPTR));
        actionDatabase->setText(QApplication::translate("MainWindow", "Database", Q_NULLPTR));
        actionSet_Music_Folder->setText(QApplication::translate("MainWindow", "Set Music Folder", Q_NULLPTR));
        actionSet_Sound_Folder->setText(QApplication::translate("MainWindow", "Set Sound Folder", Q_NULLPTR));
        actionSet_Maps_Folder->setText(QApplication::translate("MainWindow", "Set Maps Folder", Q_NULLPTR));
        actionSet_Resources_Folder->setText(QApplication::translate("MainWindow", "Set Resources Folder", Q_NULLPTR));
        actionMusic_Player->setText(QApplication::translate("MainWindow", "Music Player", Q_NULLPTR));
        actionSound_Player->setText(QApplication::translate("MainWindow", "Sound Player", Q_NULLPTR));
        actionMap_Viewer->setText(QApplication::translate("MainWindow", "Map Viewer", Q_NULLPTR));
        actionSet_Database_Path->setText(QApplication::translate("MainWindow", "Set Database Path", Q_NULLPTR));
        actionOpen_Wiki->setText(QApplication::translate("MainWindow", "Open Wiki", Q_NULLPTR));
        actionCheck_for_Updates->setText(QApplication::translate("MainWindow", "Check for Updates", Q_NULLPTR));
        actionReport_a_Bug->setText(QApplication::translate("MainWindow", "Report a Bug", Q_NULLPTR));
        actionI_want_to_use_an_older_Version->setText(QApplication::translate("MainWindow", "I want to use an older version!", Q_NULLPTR));
        actionInternet_Radio->setText(QApplication::translate("MainWindow", "Internet Radio", Q_NULLPTR));
        actionCheck_for_Updates_on_Program_Start->setText(QApplication::translate("MainWindow", "Check for Updates on Program Start", Q_NULLPTR));
        actionNamne_Generator->setText(QApplication::translate("MainWindow", "Name Generator", Q_NULLPTR));
        actionCharacters->setText(QApplication::translate("MainWindow", "Characters", Q_NULLPTR));
        musicLabel->setText(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
        musicPlayButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        musicPauseButton->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
        musicReplayButton->setText(QApplication::translate("MainWindow", "Replay", Q_NULLPTR));
        musicNextButton->setText(QApplication::translate("MainWindow", "Next", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        musicVolumeSlider->setToolTip(QApplication::translate("MainWindow", "Music Volume", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundLabel->setText(QApplication::translate("MainWindow", "Sound", Q_NULLPTR));
        soundPlayButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        soundPauseButton->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
        soundReplayButton->setText(QApplication::translate("MainWindow", "Replay", Q_NULLPTR));
        soundNextButton->setText(QApplication::translate("MainWindow", "Next", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        soundVolumeSlider->setToolTip(QApplication::translate("MainWindow", "Sound Volume", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicInfoLabel->setText(QApplication::translate("MainWindow", "Music Information", Q_NULLPTR));
        musicTitleLabel->setText(QApplication::translate("MainWindow", "<No Music Selected>", Q_NULLPTR));
        musicAlbumLabel->setText(QString());
        musicArtistLabel->setText(QString());
        musicYearLabel->setText(QString());
        soundInfoLabel->setText(QApplication::translate("MainWindow", "Sound Information", Q_NULLPTR));
        soundNameLabel->setText(QApplication::translate("MainWindow", "<No Sounds Selected>", Q_NULLPTR));
        musicCoverLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicProgressBar->setToolTip(QApplication::translate("MainWindow", "Current Music Progress", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicProgressBar->setFormat(QApplication::translate("MainWindow", "%p%", Q_NULLPTR));
        tabWidgetGMHelp->setTabText(tabWidgetGMHelp->indexOf(tabDice), QApplication::translate("MainWindow", "Dice", Q_NULLPTR));
        nameTextEdit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:20pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
        tabWidgetGMHelp->setTabText(tabWidgetGMHelp->indexOf(tabNames), QApplication::translate("MainWindow", "Names", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Characters", Q_NULLPTR));
        characterIconLabel->setText(QString());
        createCharacterButton->setText(QApplication::translate("MainWindow", "Add New", Q_NULLPTR));
        updateCharactersButton->setText(QApplication::translate("MainWindow", "Update", Q_NULLPTR));
        tabWidgetGMHelp->setTabText(tabWidgetGMHelp->indexOf(tabCharacters), QApplication::translate("MainWindow", "Characters", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "GM-Help", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">DEBUG TEXT-EDIT:</p></body></html>", Q_NULLPTR));
        tabWidgetGMHelp->setTabText(tabWidgetGMHelp->indexOf(tab_3), QApplication::translate("MainWindow", "Debug / Test Page", Q_NULLPTR));
        mapsZoomInButton->setText(QApplication::translate("MainWindow", "Zoom In", Q_NULLPTR));
        mapsZoomOutButton->setText(QApplication::translate("MainWindow", "Zoom Out", Q_NULLPTR));
        mapsFitToViewButton->setText(QApplication::translate("MainWindow", "Fit to View", Q_NULLPTR));
        mapsResetSizeButton->setText(QApplication::translate("MainWindow", "Reset Size", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Internet Radio (Beta)", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Radio Rivendell", Q_NULLPTR));
        rivendellPlayButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        rivendellReloadButton->setText(QApplication::translate("MainWindow", "Reload", Q_NULLPTR));
        label_7->setText(QString());
        textEdit_2->setDocumentTitle(QApplication::translate("MainWindow", "Infos", Q_NULLPTR));
        textEdit_2->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><title>Infos</title><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">From radiorivendell.com:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Helvetia Neue,Helvetica,Arial,Lucida Grande,Verdana,sans-serif'; font-size:16pt; font-weight:600; color:#ffffff;\">&quot;Radio Rivendell is the one and only fantasy radio station in the world playing fantasy music 24-7! We like to promote young and unknown artists and bands to the broader audience. &"
                        "quot;</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Helvetia Neue,Helvetica,Arial,Lucida Grande,Verdana,sans-serif'; font-size:16pt; font-weight:600; color:#ffffff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://www.radiorivendell.com\"><span style=\" font-size:16pt; text-decoration: underline; color:#a7cdff;\">Visit Radio Rivendell</span></a></p></body></html>", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "MMORPG Radio (German)", Q_NULLPTR));
        mmorpgPlayButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        mmorpgReloadButton->setText(QApplication::translate("MainWindow", "Reload", Q_NULLPTR));
        label_9->setText(QString());
        textEdit_3->setDocumentTitle(QApplication::translate("MainWindow", "Infos", Q_NULLPTR));
        textEdit_3->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><title>Infos</title><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">MMORPG-Radio.de:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">&quot;Das Webradio f\303\274r Gamer, Mittelalterfans und Nerds.&quot;</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:16pt; font-weight:600;\"><br /></p>\n"
"<p style"
                        "=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://mmorpg-radio.de/\"><span style=\" font-size:16pt; text-decoration: underline; color:#a7cdff;\">Visit MMORPG Radio</span></a></p></body></html>", Q_NULLPTR));
        menuGM_Help->setTitle(QApplication::translate("MainWindow", "GM-Help", Q_NULLPTR));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
        menuMusic->setTitle(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
        menuSound->setTitle(QApplication::translate("MainWindow", "Sound", Q_NULLPTR));
        menuMaps->setTitle(QApplication::translate("MainWindow", "Maps", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
