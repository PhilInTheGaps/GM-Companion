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
#include <QtWidgets/QTableView>
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
    QVBoxLayout *verticalLayout;
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
    QHBoxLayout *horizontalLayout;
    QPushButton *createCharacterButton;
    QPushButton *pushButton;
    QPushButton *updateCharactersButton;
    QStackedWidget *charactersStackedWidget;
    QWidget *widget;
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
    QWidget *pageWelcome;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_4;
    QLabel *label_10;
    QTextEdit *textEdit_4;
    QTableView *databaseView;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QLabel *musicLabel;
    QHBoxLayout *musicControls;
    QPushButton *musicPlayButton;
    QPushButton *musicPauseButton;
    QPushButton *musicReplayButton;
    QPushButton *musicNextButton;
    QPushButton *musicRandomButton;
    QSlider *musicVolumeSlider;
    QVBoxLayout *verticalLayout_18;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_19;
    QLabel *musicTitleLabel;
    QLabel *musicAlbumLabel;
    QLabel *musicArtistLabel;
    QProgressBar *musicProgressBar;
    QLabel *musicCoverLabel;
    QSpacerItem *horizontalSpacer_3;
    QFrame *line;
    QVBoxLayout *verticalLayout_17;
    QLabel *soundLabel;
    QHBoxLayout *soundControls;
    QPushButton *soundPlayButton;
    QPushButton *soundPauseButton;
    QPushButton *soundReplayButton;
    QPushButton *soundNextButton;
    QPushButton *soundRandomButton;
    QSlider *soundVolumeSlider;
    QVBoxLayout *verticalLayout_20;
    QLabel *soundNameLabel;
    QSpacerItem *horizontalSpacer_4;
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
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setFrameShape(QFrame::Box);
        stackedWidget->setFrameShadow(QFrame::Sunken);
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
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(nameTabWidget->sizePolicy().hasHeightForWidth());
        nameTabWidget->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(nameTabWidget);

        nameTextEdit = new QTextEdit(tabNames);
        nameTextEdit->setObjectName(QStringLiteral("nameTextEdit"));
        QFont font1;
        font1.setPointSize(20);
        nameTextEdit->setFont(font1);
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

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        createCharacterButton = new QPushButton(frame);
        createCharacterButton->setObjectName(QStringLiteral("createCharacterButton"));
        createCharacterButton->setCheckable(false);
        createCharacterButton->setChecked(false);

        horizontalLayout->addWidget(createCharacterButton);

        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout_5->addLayout(horizontalLayout);

        updateCharactersButton = new QPushButton(frame);
        updateCharactersButton->setObjectName(QStringLiteral("updateCharactersButton"));

        verticalLayout_5->addWidget(updateCharactersButton);


        horizontalLayout_10->addWidget(frame);

        charactersStackedWidget = new QStackedWidget(tabCharacters);
        charactersStackedWidget->setObjectName(QStringLiteral("charactersStackedWidget"));
        sizePolicy.setHeightForWidth(charactersStackedWidget->sizePolicy().hasHeightForWidth());
        charactersStackedWidget->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(charactersStackedWidget);

        widget = new QWidget(tabCharacters);
        widget->setObjectName(QStringLiteral("widget"));

        horizontalLayout_10->addWidget(widget);

        tabWidgetGMHelp->addTab(tabCharacters, QString());

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
        sizePolicy1.setHeightForWidth(mapsControlFrame->sizePolicy().hasHeightForWidth());
        mapsControlFrame->setSizePolicy(sizePolicy1);
        mapsControlFrame->setFrameShape(QFrame::StyledPanel);
        mapsControlFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(mapsControlFrame);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        mapsZoomInButton = new QPushButton(mapsControlFrame);
        mapsZoomInButton->setObjectName(QStringLiteral("mapsZoomInButton"));
        mapsZoomInButton->setMinimumSize(QSize(50, 40));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        mapsZoomInButton->setFont(font2);

        verticalLayout_4->addWidget(mapsZoomInButton);

        mapsZoomOutButton = new QPushButton(mapsControlFrame);
        mapsZoomOutButton->setObjectName(QStringLiteral("mapsZoomOutButton"));
        mapsZoomOutButton->setMinimumSize(QSize(50, 40));
        mapsZoomOutButton->setFont(font2);

        verticalLayout_4->addWidget(mapsZoomOutButton);

        mapsFitToViewButton = new QPushButton(mapsControlFrame);
        mapsFitToViewButton->setObjectName(QStringLiteral("mapsFitToViewButton"));
        mapsFitToViewButton->setMinimumSize(QSize(50, 40));
        mapsFitToViewButton->setFont(font2);

        verticalLayout_4->addWidget(mapsFitToViewButton);

        mapsResetSizeButton = new QPushButton(mapsControlFrame);
        mapsResetSizeButton->setObjectName(QStringLiteral("mapsResetSizeButton"));
        mapsResetSizeButton->setMinimumSize(QSize(50, 40));
        mapsResetSizeButton->setFont(font2);

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
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        QFont font3;
        font3.setPointSize(30);
        label_5->setFont(font3);

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
        sizePolicy2.setHeightForWidth(radioRivendellFrame->sizePolicy().hasHeightForWidth());
        radioRivendellFrame->setSizePolicy(sizePolicy2);
        radioRivendellFrame->setMaximumSize(QSize(1000, 450));
        radioRivendellFrame->setFrameShape(QFrame::Box);
        radioRivendellFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_12 = new QVBoxLayout(radioRivendellFrame);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        label_6 = new QLabel(radioRivendellFrame);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font1);

        verticalLayout_12->addWidget(label_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        rivendellPlayButton = new QPushButton(radioRivendellFrame);
        rivendellPlayButton->setObjectName(QStringLiteral("rivendellPlayButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(rivendellPlayButton->sizePolicy().hasHeightForWidth());
        rivendellPlayButton->setSizePolicy(sizePolicy3);
        rivendellPlayButton->setMinimumSize(QSize(90, 40));
        rivendellPlayButton->setMaximumSize(QSize(100, 16777215));
        QFont font4;
        font4.setPointSize(15);
        font4.setBold(false);
        font4.setWeight(50);
        rivendellPlayButton->setFont(font4);
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
        sizePolicy3.setHeightForWidth(rivendellReloadButton->sizePolicy().hasHeightForWidth());
        rivendellReloadButton->setSizePolicy(sizePolicy3);
        rivendellReloadButton->setMinimumSize(QSize(90, 40));
        rivendellReloadButton->setMaximumSize(QSize(100, 16777215));
        rivendellReloadButton->setFont(font4);
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
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy4);
        label_7->setMinimumSize(QSize(300, 0));
        label_7->setMaximumSize(QSize(820, 2000));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/resources/radio/logo-radio-rivendell.png")));
        label_7->setScaledContents(true);

        verticalLayout_11->addWidget(label_7);

        textEdit_2 = new QTextEdit(radioRivendellFrame);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(textEdit_2->sizePolicy().hasHeightForWidth());
        textEdit_2->setSizePolicy(sizePolicy5);
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
        sizePolicy2.setHeightForWidth(radioMmorpgDeFrame->sizePolicy().hasHeightForWidth());
        radioMmorpgDeFrame->setSizePolicy(sizePolicy2);
        radioMmorpgDeFrame->setMaximumSize(QSize(1000, 450));
        radioMmorpgDeFrame->setFrameShape(QFrame::Box);
        radioMmorpgDeFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_14 = new QVBoxLayout(radioMmorpgDeFrame);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        label_8 = new QLabel(radioMmorpgDeFrame);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);

        verticalLayout_14->addWidget(label_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        mmorpgPlayButton = new QPushButton(radioMmorpgDeFrame);
        mmorpgPlayButton->setObjectName(QStringLiteral("mmorpgPlayButton"));
        sizePolicy3.setHeightForWidth(mmorpgPlayButton->sizePolicy().hasHeightForWidth());
        mmorpgPlayButton->setSizePolicy(sizePolicy3);
        mmorpgPlayButton->setMinimumSize(QSize(90, 40));
        mmorpgPlayButton->setMaximumSize(QSize(100, 16777215));
        QFont font5;
        font5.setPointSize(15);
        mmorpgPlayButton->setFont(font5);
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
        sizePolicy3.setHeightForWidth(mmorpgReloadButton->sizePolicy().hasHeightForWidth());
        mmorpgReloadButton->setSizePolicy(sizePolicy3);
        mmorpgReloadButton->setMinimumSize(QSize(90, 40));
        mmorpgReloadButton->setMaximumSize(QSize(100, 16777215));
        mmorpgReloadButton->setFont(font5);
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
        sizePolicy4.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy4);
        label_9->setMaximumSize(QSize(820, 16777215));
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/resources/radio/logo-mmorp-radio-de.png")));
        label_9->setScaledContents(true);

        verticalLayout_16->addWidget(label_9);

        textEdit_3 = new QTextEdit(radioMmorpgDeFrame);
        textEdit_3->setObjectName(QStringLiteral("textEdit_3"));
        sizePolicy5.setHeightForWidth(textEdit_3->sizePolicy().hasHeightForWidth());
        textEdit_3->setSizePolicy(sizePolicy5);
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
        pageWelcome = new QWidget();
        pageWelcome->setObjectName(QStringLiteral("pageWelcome"));
        verticalLayout_7 = new QVBoxLayout(pageWelcome);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_3 = new QLabel(pageWelcome);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(16777215, 100));
        QFont font6;
        font6.setPointSize(50);
        font6.setBold(false);
        font6.setItalic(false);
        font6.setUnderline(false);
        font6.setWeight(50);
        font6.setStrikeOut(false);
        font6.setKerning(true);
        font6.setStyleStrategy(QFont::PreferAntialias);
        label_3->setFont(font6);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_7->addWidget(label_3);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_4 = new QLabel(pageWelcome);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_8->addWidget(label_4);

        label_10 = new QLabel(pageWelcome);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_8->addWidget(label_10);

        textEdit_4 = new QTextEdit(pageWelcome);
        textEdit_4->setObjectName(QStringLiteral("textEdit_4"));

        verticalLayout_8->addWidget(textEdit_4);

        databaseView = new QTableView(pageWelcome);
        databaseView->setObjectName(QStringLiteral("databaseView"));

        verticalLayout_8->addWidget(databaseView);


        horizontalLayout_11->addLayout(verticalLayout_8);


        verticalLayout_7->addLayout(horizontalLayout_11);

        stackedWidget->addWidget(pageWelcome);

        verticalLayout->addWidget(stackedWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        musicLabel = new QLabel(centralWidget);
        musicLabel->setObjectName(QStringLiteral("musicLabel"));
        sizePolicy2.setHeightForWidth(musicLabel->sizePolicy().hasHeightForWidth());
        musicLabel->setSizePolicy(sizePolicy2);
        QFont font7;
        font7.setPointSize(8);
        font7.setBold(false);
        font7.setWeight(50);
        musicLabel->setFont(font7);

        verticalLayout_6->addWidget(musicLabel);

        musicControls = new QHBoxLayout();
        musicControls->setSpacing(6);
        musicControls->setObjectName(QStringLiteral("musicControls"));
        musicControls->setSizeConstraint(QLayout::SetDefaultConstraint);
        musicPlayButton = new QPushButton(centralWidget);
        musicPlayButton->setObjectName(QStringLiteral("musicPlayButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(musicPlayButton->sizePolicy().hasHeightForWidth());
        musicPlayButton->setSizePolicy(sizePolicy6);
        musicPlayButton->setMinimumSize(QSize(50, 40));
        musicPlayButton->setMaximumSize(QSize(68, 45));
        QFont font8;
        font8.setPointSize(10);
        font8.setBold(true);
        font8.setWeight(75);
        musicPlayButton->setFont(font8);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resources/mediaIcons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        musicPlayButton->setIcon(icon1);
        musicPlayButton->setIconSize(QSize(32, 32));
        musicPlayButton->setAutoDefault(false);
        musicPlayButton->setFlat(false);

        musicControls->addWidget(musicPlayButton);

        musicPauseButton = new QPushButton(centralWidget);
        musicPauseButton->setObjectName(QStringLiteral("musicPauseButton"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(musicPauseButton->sizePolicy().hasHeightForWidth());
        musicPauseButton->setSizePolicy(sizePolicy7);
        musicPauseButton->setMinimumSize(QSize(50, 40));
        musicPauseButton->setMaximumSize(QSize(68, 45));
        musicPauseButton->setFont(font8);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/resources/mediaIcons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        musicPauseButton->setIcon(icon2);
        musicPauseButton->setIconSize(QSize(32, 32));

        musicControls->addWidget(musicPauseButton);

        musicReplayButton = new QPushButton(centralWidget);
        musicReplayButton->setObjectName(QStringLiteral("musicReplayButton"));
        sizePolicy7.setHeightForWidth(musicReplayButton->sizePolicy().hasHeightForWidth());
        musicReplayButton->setSizePolicy(sizePolicy7);
        musicReplayButton->setMinimumSize(QSize(50, 40));
        musicReplayButton->setMaximumSize(QSize(68, 45));
        musicReplayButton->setFont(font8);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/resources/mediaIcons/skipBack.png"), QSize(), QIcon::Normal, QIcon::Off);
        musicReplayButton->setIcon(icon3);
        musicReplayButton->setIconSize(QSize(32, 32));

        musicControls->addWidget(musicReplayButton);

        musicNextButton = new QPushButton(centralWidget);
        musicNextButton->setObjectName(QStringLiteral("musicNextButton"));
        sizePolicy7.setHeightForWidth(musicNextButton->sizePolicy().hasHeightForWidth());
        musicNextButton->setSizePolicy(sizePolicy7);
        musicNextButton->setMinimumSize(QSize(50, 40));
        musicNextButton->setMaximumSize(QSize(68, 45));
        musicNextButton->setFont(font8);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/resources/mediaIcons/seekForward.png"), QSize(), QIcon::Normal, QIcon::Off);
        musicNextButton->setIcon(icon4);
        musicNextButton->setIconSize(QSize(32, 32));

        musicControls->addWidget(musicNextButton);

        musicRandomButton = new QPushButton(centralWidget);
        musicRandomButton->setObjectName(QStringLiteral("musicRandomButton"));
        sizePolicy7.setHeightForWidth(musicRandomButton->sizePolicy().hasHeightForWidth());
        musicRandomButton->setSizePolicy(sizePolicy7);
        musicRandomButton->setMaximumSize(QSize(68, 45));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/resources/mediaIcons/shuffleOn.png"), QSize(), QIcon::Normal, QIcon::Off);
        musicRandomButton->setIcon(icon5);
        musicRandomButton->setIconSize(QSize(32, 32));
        musicRandomButton->setCheckable(true);
        musicRandomButton->setChecked(true);

        musicControls->addWidget(musicRandomButton);


        verticalLayout_6->addLayout(musicControls);

        musicVolumeSlider = new QSlider(centralWidget);
        musicVolumeSlider->setObjectName(QStringLiteral("musicVolumeSlider"));
        QSizePolicy sizePolicy8(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(musicVolumeSlider->sizePolicy().hasHeightForWidth());
        musicVolumeSlider->setSizePolicy(sizePolicy8);
        musicVolumeSlider->setMinimumSize(QSize(300, 0));
        musicVolumeSlider->setValue(20);
        musicVolumeSlider->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(musicVolumeSlider);


        horizontalLayout_2->addLayout(verticalLayout_6);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        musicTitleLabel = new QLabel(centralWidget);
        musicTitleLabel->setObjectName(QStringLiteral("musicTitleLabel"));
        sizePolicy2.setHeightForWidth(musicTitleLabel->sizePolicy().hasHeightForWidth());
        musicTitleLabel->setSizePolicy(sizePolicy2);
        musicTitleLabel->setMaximumSize(QSize(300, 16777215));
        musicTitleLabel->setFont(font7);
        musicTitleLabel->setWordWrap(false);

        verticalLayout_19->addWidget(musicTitleLabel);

        musicAlbumLabel = new QLabel(centralWidget);
        musicAlbumLabel->setObjectName(QStringLiteral("musicAlbumLabel"));
        sizePolicy2.setHeightForWidth(musicAlbumLabel->sizePolicy().hasHeightForWidth());
        musicAlbumLabel->setSizePolicy(sizePolicy2);
        musicAlbumLabel->setMaximumSize(QSize(300, 16777215));
        musicAlbumLabel->setFont(font7);
        musicAlbumLabel->setWordWrap(false);

        verticalLayout_19->addWidget(musicAlbumLabel);

        musicArtistLabel = new QLabel(centralWidget);
        musicArtistLabel->setObjectName(QStringLiteral("musicArtistLabel"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(musicArtistLabel->sizePolicy().hasHeightForWidth());
        musicArtistLabel->setSizePolicy(sizePolicy9);
        musicArtistLabel->setMaximumSize(QSize(300, 16777215));
        musicArtistLabel->setFont(font7);
        musicArtistLabel->setWordWrap(false);

        verticalLayout_19->addWidget(musicArtistLabel);


        horizontalLayout_12->addLayout(verticalLayout_19);


        verticalLayout_18->addLayout(horizontalLayout_12);

        musicProgressBar = new QProgressBar(centralWidget);
        musicProgressBar->setObjectName(QStringLiteral("musicProgressBar"));
        QSizePolicy sizePolicy10(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(musicProgressBar->sizePolicy().hasHeightForWidth());
        musicProgressBar->setSizePolicy(sizePolicy10);
        musicProgressBar->setMinimumSize(QSize(300, 0));
        musicProgressBar->setMaximumSize(QSize(300, 15));
        musicProgressBar->setAutoFillBackground(false);
        musicProgressBar->setStyleSheet(QStringLiteral(""));
        musicProgressBar->setMaximum(1);
        musicProgressBar->setValue(0);
        musicProgressBar->setTextVisible(true);
        musicProgressBar->setTextDirection(QProgressBar::TopToBottom);

        verticalLayout_18->addWidget(musicProgressBar);


        horizontalLayout_2->addLayout(verticalLayout_18);

        musicCoverLabel = new QLabel(centralWidget);
        musicCoverLabel->setObjectName(QStringLiteral("musicCoverLabel"));
        QSizePolicy sizePolicy11(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy11.setHorizontalStretch(0);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(musicCoverLabel->sizePolicy().hasHeightForWidth());
        musicCoverLabel->setSizePolicy(sizePolicy11);
        musicCoverLabel->setMinimumSize(QSize(85, 85));
        musicCoverLabel->setMaximumSize(QSize(85, 85));
        QFont font9;
        font9.setPointSize(8);
        musicCoverLabel->setFont(font9);
        musicCoverLabel->setFrameShape(QFrame::NoFrame);
        musicCoverLabel->setFrameShadow(QFrame::Plain);

        horizontalLayout_2->addWidget(musicCoverLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        soundLabel = new QLabel(centralWidget);
        soundLabel->setObjectName(QStringLiteral("soundLabel"));
        sizePolicy2.setHeightForWidth(soundLabel->sizePolicy().hasHeightForWidth());
        soundLabel->setSizePolicy(sizePolicy2);
        soundLabel->setFont(font7);

        verticalLayout_17->addWidget(soundLabel);

        soundControls = new QHBoxLayout();
        soundControls->setSpacing(6);
        soundControls->setObjectName(QStringLiteral("soundControls"));
        soundPlayButton = new QPushButton(centralWidget);
        soundPlayButton->setObjectName(QStringLiteral("soundPlayButton"));
        sizePolicy7.setHeightForWidth(soundPlayButton->sizePolicy().hasHeightForWidth());
        soundPlayButton->setSizePolicy(sizePolicy7);
        soundPlayButton->setMinimumSize(QSize(50, 40));
        soundPlayButton->setMaximumSize(QSize(68, 45));
        soundPlayButton->setFont(font8);
        soundPlayButton->setIcon(icon1);
        soundPlayButton->setIconSize(QSize(32, 32));

        soundControls->addWidget(soundPlayButton);

        soundPauseButton = new QPushButton(centralWidget);
        soundPauseButton->setObjectName(QStringLiteral("soundPauseButton"));
        sizePolicy7.setHeightForWidth(soundPauseButton->sizePolicy().hasHeightForWidth());
        soundPauseButton->setSizePolicy(sizePolicy7);
        soundPauseButton->setMinimumSize(QSize(50, 40));
        soundPauseButton->setMaximumSize(QSize(68, 45));
        soundPauseButton->setFont(font8);
        soundPauseButton->setIcon(icon2);
        soundPauseButton->setIconSize(QSize(32, 32));

        soundControls->addWidget(soundPauseButton);

        soundReplayButton = new QPushButton(centralWidget);
        soundReplayButton->setObjectName(QStringLiteral("soundReplayButton"));
        sizePolicy7.setHeightForWidth(soundReplayButton->sizePolicy().hasHeightForWidth());
        soundReplayButton->setSizePolicy(sizePolicy7);
        soundReplayButton->setMinimumSize(QSize(50, 40));
        soundReplayButton->setMaximumSize(QSize(68, 45));
        soundReplayButton->setFont(font8);
        soundReplayButton->setIcon(icon3);
        soundReplayButton->setIconSize(QSize(32, 32));

        soundControls->addWidget(soundReplayButton);

        soundNextButton = new QPushButton(centralWidget);
        soundNextButton->setObjectName(QStringLiteral("soundNextButton"));
        sizePolicy7.setHeightForWidth(soundNextButton->sizePolicy().hasHeightForWidth());
        soundNextButton->setSizePolicy(sizePolicy7);
        soundNextButton->setMinimumSize(QSize(50, 40));
        soundNextButton->setMaximumSize(QSize(68, 45));
        soundNextButton->setFont(font8);
        soundNextButton->setIcon(icon4);
        soundNextButton->setIconSize(QSize(32, 32));

        soundControls->addWidget(soundNextButton);

        soundRandomButton = new QPushButton(centralWidget);
        soundRandomButton->setObjectName(QStringLiteral("soundRandomButton"));
        sizePolicy7.setHeightForWidth(soundRandomButton->sizePolicy().hasHeightForWidth());
        soundRandomButton->setSizePolicy(sizePolicy7);
        soundRandomButton->setMaximumSize(QSize(68, 45));
        soundRandomButton->setIcon(icon5);
        soundRandomButton->setIconSize(QSize(32, 32));
        soundRandomButton->setCheckable(true);
        soundRandomButton->setChecked(true);

        soundControls->addWidget(soundRandomButton);


        verticalLayout_17->addLayout(soundControls);

        soundVolumeSlider = new QSlider(centralWidget);
        soundVolumeSlider->setObjectName(QStringLiteral("soundVolumeSlider"));
        sizePolicy8.setHeightForWidth(soundVolumeSlider->sizePolicy().hasHeightForWidth());
        soundVolumeSlider->setSizePolicy(sizePolicy8);
        soundVolumeSlider->setMinimumSize(QSize(300, 0));
        soundVolumeSlider->setValue(20);
        soundVolumeSlider->setOrientation(Qt::Horizontal);

        verticalLayout_17->addWidget(soundVolumeSlider);


        horizontalLayout_2->addLayout(verticalLayout_17);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        soundNameLabel = new QLabel(centralWidget);
        soundNameLabel->setObjectName(QStringLiteral("soundNameLabel"));
        QSizePolicy sizePolicy12(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy12.setHorizontalStretch(0);
        sizePolicy12.setVerticalStretch(0);
        sizePolicy12.setHeightForWidth(soundNameLabel->sizePolicy().hasHeightForWidth());
        soundNameLabel->setSizePolicy(sizePolicy12);
        soundNameLabel->setMaximumSize(QSize(300, 85));
        soundNameLabel->setFont(font9);
        soundNameLabel->setWordWrap(true);

        verticalLayout_20->addWidget(soundNameLabel);


        horizontalLayout_2->addLayout(verticalLayout_20);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1300, 28));
        QSizePolicy sizePolicy13(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy13.setHorizontalStretch(0);
        sizePolicy13.setVerticalStretch(0);
        sizePolicy13.setHeightForWidth(menuBar->sizePolicy().hasHeightForWidth());
        menuBar->setSizePolicy(sizePolicy13);
        menuBar->setMinimumSize(QSize(0, 25));
        menuBar->setBaseSize(QSize(0, 25));
        QFont font10;
        font10.setPointSize(11);
        font10.setBold(true);
        font10.setWeight(75);
        menuBar->setFont(font10);
        menuBar->setStyleSheet(QStringLiteral(""));
        menuGM_Help = new QMenu(menuBar);
        menuGM_Help->setObjectName(QStringLiteral("menuGM_Help"));
        QFont font11;
        font11.setPointSize(10);
        font11.setBold(false);
        font11.setWeight(50);
        menuGM_Help->setFont(font11);
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuOptions->setFont(font11);
        menuMusic = new QMenu(menuBar);
        menuMusic->setObjectName(QStringLiteral("menuMusic"));
        menuMusic->setFont(font11);
        menuSound = new QMenu(menuBar);
        menuSound->setObjectName(QStringLiteral("menuSound"));
        menuSound->setFont(font11);
        menuMaps = new QMenu(menuBar);
        menuMaps->setObjectName(QStringLiteral("menuMaps"));
        menuMaps->setFont(font11);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        QFont font12;
        font12.setPointSize(10);
        menuHelp->setFont(font12);
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

        stackedWidget->setCurrentIndex(5);
        tabWidgetGMHelp->setCurrentIndex(0);
        musicPlayButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GM-Companion | DEV-BUILD Beta 3.0 PRE 06", Q_NULLPTR));
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
        pushButton->setText(QApplication::translate("MainWindow", "Edit Selected", Q_NULLPTR));
        updateCharactersButton->setText(QApplication::translate("MainWindow", "Update", Q_NULLPTR));
        tabWidgetGMHelp->setTabText(tabWidgetGMHelp->indexOf(tabCharacters), QApplication::translate("MainWindow", "Characters", Q_NULLPTR));
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
        label_3->setText(QApplication::translate("MainWindow", "Welcome!", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Version: Beta 3.0", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Changelog:", Q_NULLPTR));
        textEdit_4->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Changed media control buttons to have icons instead of text</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added support for webradios</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added music controls when hovering icon in taskbar</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added name generator</p>\n"
""
                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added character overview</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Improved Maps-Viewer with zoom function</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Greatly improved memory usage</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added function to check for updates</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added function to open wiki</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- Added function to report bugs</p></body></html>", Q_NULLPTR));
        musicLabel->setText(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        musicPlayButton->setToolTip(QApplication::translate("MainWindow", "Play (Music)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicPlayButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicPauseButton->setToolTip(QApplication::translate("MainWindow", "Pause (Music)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicPauseButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicReplayButton->setToolTip(QApplication::translate("MainWindow", "Rewind (Music)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicReplayButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicNextButton->setToolTip(QApplication::translate("MainWindow", "Next (Music)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicNextButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicRandomButton->setToolTip(QApplication::translate("MainWindow", "Toggle Music Shuffle (bright = on, dark = off)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicRandomButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicVolumeSlider->setToolTip(QApplication::translate("MainWindow", "Music Volume", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicTitleLabel->setText(QApplication::translate("MainWindow", "<No Music Selected>", Q_NULLPTR));
        musicAlbumLabel->setText(QString());
        musicArtistLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicProgressBar->setToolTip(QApplication::translate("MainWindow", "Current Music Progress", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicProgressBar->setFormat(QApplication::translate("MainWindow", "%p%", Q_NULLPTR));
        musicCoverLabel->setText(QString());
        soundLabel->setText(QApplication::translate("MainWindow", "Sound", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        soundPlayButton->setToolTip(QApplication::translate("MainWindow", "Play (Sound)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundPlayButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        soundPauseButton->setToolTip(QApplication::translate("MainWindow", "Pause (Sound)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundPauseButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        soundReplayButton->setToolTip(QApplication::translate("MainWindow", "Rewind (Sound)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundReplayButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        soundNextButton->setToolTip(QApplication::translate("MainWindow", "Next (Sound)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundNextButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        soundRandomButton->setToolTip(QApplication::translate("MainWindow", "Toggle Sound Shuffle (bright = on, dark = off)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundRandomButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        soundVolumeSlider->setToolTip(QApplication::translate("MainWindow", "Sound Volume", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        soundNameLabel->setText(QApplication::translate("MainWindow", "<No Sounds Selected>", Q_NULLPTR));
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
