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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
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
    QTabWidget *tabWidget;
    QWidget *tabMusic;
    QWidget *tabSound;
    QWidget *tabMaps;
    QHBoxLayout *horizontalLayout;
    QFrame *mapsControlFrame;
    QVBoxLayout *verticalLayout_4;
    QPushButton *mapsZoomInButton;
    QPushButton *mapsZoomOutButton;
    QPushButton *mapsFitToViewButton;
    QPushButton *mapsResetSizeButton;
    QFrame *line_5;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *mapsVBox;
    QFrame *mapsButtonFrame;
    QWidget *tab;
    QVBoxLayout *verticalLayout_6;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuGM_Help;
    QMenu *menuOptions;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1300, 727);
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
"QPushButton:hover:!pressed{\n"
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
"    width: 1px"
                        ";\n"
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
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
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
""
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
"    hborder: 5px solid rgba(68, 119, 170, 150);\n"
"	\n"
"	background-color: #333333;\n"
"}\n"
"\n"
"QTableView::item:selected {\n"
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
"QScrollBar::sub-line:"
                        "horizontal {\n"
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
"    background-color: #222222;\n"
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
        font.setBold(false);
        font.setWeight(50);
        actionDice->setFont(font);
        actionDatabase = new QAction(MainWindow);
        actionDatabase->setObjectName(QStringLiteral("actionDatabase"));
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
        QFont font3;
        font3.setPointSize(11);
        musicTitleLabel->setFont(font3);

        infoBox->addWidget(musicTitleLabel);

        musicAlbumLabel = new QLabel(centralWidget);
        musicAlbumLabel->setObjectName(QStringLiteral("musicAlbumLabel"));
        sizePolicy1.setHeightForWidth(musicAlbumLabel->sizePolicy().hasHeightForWidth());
        musicAlbumLabel->setSizePolicy(sizePolicy1);
        musicAlbumLabel->setFont(font3);

        infoBox->addWidget(musicAlbumLabel);

        musicArtistLabel = new QLabel(centralWidget);
        musicArtistLabel->setObjectName(QStringLiteral("musicArtistLabel"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(musicArtistLabel->sizePolicy().hasHeightForWidth());
        musicArtistLabel->setSizePolicy(sizePolicy6);
        musicArtistLabel->setFont(font3);

        infoBox->addWidget(musicArtistLabel);

        musicYearLabel = new QLabel(centralWidget);
        musicYearLabel->setObjectName(QStringLiteral("musicYearLabel"));
        sizePolicy6.setHeightForWidth(musicYearLabel->sizePolicy().hasHeightForWidth());
        musicYearLabel->setSizePolicy(sizePolicy6);
        musicYearLabel->setFont(font3);

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
        soundNameLabel->setFont(font3);

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

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabMusic = new QWidget();
        tabMusic->setObjectName(QStringLiteral("tabMusic"));
        tabWidget->addTab(tabMusic, QString());
        tabSound = new QWidget();
        tabSound->setObjectName(QStringLiteral("tabSound"));
        tabWidget->addTab(tabSound, QString());
        tabMaps = new QWidget();
        tabMaps->setObjectName(QStringLiteral("tabMaps"));
        horizontalLayout = new QHBoxLayout(tabMaps);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mapsControlFrame = new QFrame(tabMaps);
        mapsControlFrame->setObjectName(QStringLiteral("mapsControlFrame"));
        QSizePolicy sizePolicy8(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
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
        QFont font4;
        font4.setBold(true);
        font4.setWeight(75);
        mapsZoomInButton->setFont(font4);

        verticalLayout_4->addWidget(mapsZoomInButton);

        mapsZoomOutButton = new QPushButton(mapsControlFrame);
        mapsZoomOutButton->setObjectName(QStringLiteral("mapsZoomOutButton"));
        mapsZoomOutButton->setMinimumSize(QSize(50, 40));
        mapsZoomOutButton->setFont(font4);

        verticalLayout_4->addWidget(mapsZoomOutButton);

        mapsFitToViewButton = new QPushButton(mapsControlFrame);
        mapsFitToViewButton->setObjectName(QStringLiteral("mapsFitToViewButton"));
        mapsFitToViewButton->setMinimumSize(QSize(50, 40));
        mapsFitToViewButton->setFont(font4);

        verticalLayout_4->addWidget(mapsFitToViewButton);

        mapsResetSizeButton = new QPushButton(mapsControlFrame);
        mapsResetSizeButton->setObjectName(QStringLiteral("mapsResetSizeButton"));
        mapsResetSizeButton->setMinimumSize(QSize(50, 40));
        mapsResetSizeButton->setFont(font4);

        verticalLayout_4->addWidget(mapsResetSizeButton);

        line_5 = new QFrame(mapsControlFrame);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        horizontalLayout->addWidget(mapsControlFrame);

        mapsVBox = new QVBoxLayout();
        mapsVBox->setSpacing(6);
        mapsVBox->setObjectName(QStringLiteral("mapsVBox"));
        mapsButtonFrame = new QFrame(tabMaps);
        mapsButtonFrame->setObjectName(QStringLiteral("mapsButtonFrame"));
        QSizePolicy sizePolicy9(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(mapsButtonFrame->sizePolicy().hasHeightForWidth());
        mapsButtonFrame->setSizePolicy(sizePolicy9);
        mapsButtonFrame->setFrameShape(QFrame::StyledPanel);
        mapsButtonFrame->setFrameShadow(QFrame::Raised);

        mapsVBox->addWidget(mapsButtonFrame);


        horizontalLayout->addLayout(mapsVBox);

        tabWidget->addTab(tabMaps, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_6 = new QVBoxLayout(tab);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        textEdit = new QTextEdit(tab);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout_6->addWidget(textEdit);

        tabWidget->addTab(tab, QString());

        horizontalLayout_2->addWidget(tabWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1300, 25));
        QSizePolicy sizePolicy10(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(menuBar->sizePolicy().hasHeightForWidth());
        menuBar->setSizePolicy(sizePolicy10);
        menuBar->setMinimumSize(QSize(0, 25));
        menuBar->setBaseSize(QSize(0, 25));
        menuBar->setFont(font4);
        menuBar->setStyleSheet(QStringLiteral(""));
        menuGM_Help = new QMenu(menuBar);
        menuGM_Help->setObjectName(QStringLiteral("menuGM_Help"));
        QFont font5;
        font5.setPointSize(12);
        font5.setBold(true);
        font5.setWeight(75);
        menuGM_Help->setFont(font5);
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuOptions->setFont(font5);
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGM_Help->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuGM_Help->addAction(actionDice);
        menuGM_Help->addAction(actionDatabase);
        menuOptions->addAction(actionSet_Music_Folder);
        menuOptions->addAction(actionSet_Sound_Folder);
        menuOptions->addAction(actionSet_Maps_Folder);
        menuOptions->addAction(actionSet_Resources_Folder);

        retranslateUi(MainWindow);

        musicPlayButton->setDefault(false);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GM-Companion | DEV-BUILD Beta 3.0 PRE 01", Q_NULLPTR));
        actionDice->setText(QApplication::translate("MainWindow", "Dice", Q_NULLPTR));
        actionDatabase->setText(QApplication::translate("MainWindow", "Database", Q_NULLPTR));
        actionSet_Music_Folder->setText(QApplication::translate("MainWindow", "Set Music Folder", Q_NULLPTR));
        actionSet_Sound_Folder->setText(QApplication::translate("MainWindow", "Set Sound Folder", Q_NULLPTR));
        actionSet_Maps_Folder->setText(QApplication::translate("MainWindow", "Set Maps Folder", Q_NULLPTR));
        actionSet_Resources_Folder->setText(QApplication::translate("MainWindow", "Set Resources Folder", Q_NULLPTR));
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
        musicTitleLabel->setText(QApplication::translate("MainWindow", "Title", Q_NULLPTR));
        musicAlbumLabel->setText(QApplication::translate("MainWindow", "Album", Q_NULLPTR));
        musicArtistLabel->setText(QApplication::translate("MainWindow", "Artist", Q_NULLPTR));
        musicYearLabel->setText(QApplication::translate("MainWindow", "Year", Q_NULLPTR));
        soundInfoLabel->setText(QApplication::translate("MainWindow", "Sound Information", Q_NULLPTR));
        soundNameLabel->setText(QApplication::translate("MainWindow", "Name", Q_NULLPTR));
        musicCoverLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        musicProgressBar->setToolTip(QApplication::translate("MainWindow", "Current Music Progress", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        musicProgressBar->setFormat(QApplication::translate("MainWindow", "%p%", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        tabMusic->setToolTip(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_ACCESSIBILITY
        tabMusic->setAccessibleName(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
#endif // QT_NO_ACCESSIBILITY
        tabWidget->setTabText(tabWidget->indexOf(tabMusic), QApplication::translate("MainWindow", "Music", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSound), QApplication::translate("MainWindow", "Sound", Q_NULLPTR));
        mapsZoomInButton->setText(QApplication::translate("MainWindow", "Zoom In", Q_NULLPTR));
        mapsZoomOutButton->setText(QApplication::translate("MainWindow", "Zoom Out", Q_NULLPTR));
        mapsFitToViewButton->setText(QApplication::translate("MainWindow", "Fit to View", Q_NULLPTR));
        mapsResetSizeButton->setText(QApplication::translate("MainWindow", "Reset Size", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabMaps), QApplication::translate("MainWindow", "Maps", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Test Text</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "DEBUG", Q_NULLPTR));
        menuGM_Help->setTitle(QApplication::translate("MainWindow", "GM-Help", Q_NULLPTR));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
