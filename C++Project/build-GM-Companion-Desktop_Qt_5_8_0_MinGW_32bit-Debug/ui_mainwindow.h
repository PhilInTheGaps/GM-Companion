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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDice;
    QAction *actionDatabase;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *infoBox;
    QLabel *musicLabel;
    QHBoxLayout *musicControls;
    QPushButton *musicPlayButton;
    QPushButton *musicReplayButton;
    QPushButton *musicNextButton;
    QSlider *musicVolumeSlider;
    QFrame *line_2;
    QLabel *soundLabel;
    QHBoxLayout *soundControls;
    QPushButton *soundPlayButton;
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
    QGraphicsView *coverView;
    QTabWidget *tabWidget;
    QWidget *tabMusic;
    QVBoxLayout *verticalLayout_3;
    QToolBox *toolBox;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_4;
    QTreeWidget *treeWidget;
    QWidget *tabSound;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuGM_Help;
    QMenu *menuOptions;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 720);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/ico/resources/icon256.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setTabShape(QTabWidget::Triangular);
        actionDice = new QAction(MainWindow);
        actionDice->setObjectName(QStringLiteral("actionDice"));
        actionDatabase = new QAction(MainWindow);
        actionDatabase->setObjectName(QStringLiteral("actionDatabase"));
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

        infoBox->addWidget(musicLabel);

        musicControls = new QHBoxLayout();
        musicControls->setSpacing(6);
        musicControls->setObjectName(QStringLiteral("musicControls"));
        musicControls->setSizeConstraint(QLayout::SetDefaultConstraint);
        musicPlayButton = new QPushButton(centralWidget);
        musicPlayButton->setObjectName(QStringLiteral("musicPlayButton"));

        musicControls->addWidget(musicPlayButton);

        musicReplayButton = new QPushButton(centralWidget);
        musicReplayButton->setObjectName(QStringLiteral("musicReplayButton"));

        musicControls->addWidget(musicReplayButton);

        musicNextButton = new QPushButton(centralWidget);
        musicNextButton->setObjectName(QStringLiteral("musicNextButton"));

        musicControls->addWidget(musicNextButton);


        infoBox->addLayout(musicControls);

        musicVolumeSlider = new QSlider(centralWidget);
        musicVolumeSlider->setObjectName(QStringLiteral("musicVolumeSlider"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(musicVolumeSlider->sizePolicy().hasHeightForWidth());
        musicVolumeSlider->setSizePolicy(sizePolicy2);
        musicVolumeSlider->setMinimumSize(QSize(300, 0));
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

        infoBox->addWidget(soundLabel);

        soundControls = new QHBoxLayout();
        soundControls->setSpacing(6);
        soundControls->setObjectName(QStringLiteral("soundControls"));
        soundPlayButton = new QPushButton(centralWidget);
        soundPlayButton->setObjectName(QStringLiteral("soundPlayButton"));

        soundControls->addWidget(soundPlayButton);

        soundReplayButton = new QPushButton(centralWidget);
        soundReplayButton->setObjectName(QStringLiteral("soundReplayButton"));

        soundControls->addWidget(soundReplayButton);

        soundNextButton = new QPushButton(centralWidget);
        soundNextButton->setObjectName(QStringLiteral("soundNextButton"));

        soundControls->addWidget(soundNextButton);


        infoBox->addLayout(soundControls);

        soundVolumeSlider = new QSlider(centralWidget);
        soundVolumeSlider->setObjectName(QStringLiteral("soundVolumeSlider"));
        sizePolicy2.setHeightForWidth(soundVolumeSlider->sizePolicy().hasHeightForWidth());
        soundVolumeSlider->setSizePolicy(sizePolicy2);
        soundVolumeSlider->setMinimumSize(QSize(300, 0));
        soundVolumeSlider->setOrientation(Qt::Horizontal);

        infoBox->addWidget(soundVolumeSlider);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy3);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line);

        musicInfoLabel = new QLabel(centralWidget);
        musicInfoLabel->setObjectName(QStringLiteral("musicInfoLabel"));
        sizePolicy1.setHeightForWidth(musicInfoLabel->sizePolicy().hasHeightForWidth());
        musicInfoLabel->setSizePolicy(sizePolicy1);

        infoBox->addWidget(musicInfoLabel);

        musicTitleLabel = new QLabel(centralWidget);
        musicTitleLabel->setObjectName(QStringLiteral("musicTitleLabel"));
        sizePolicy1.setHeightForWidth(musicTitleLabel->sizePolicy().hasHeightForWidth());
        musicTitleLabel->setSizePolicy(sizePolicy1);

        infoBox->addWidget(musicTitleLabel);

        musicAlbumLabel = new QLabel(centralWidget);
        musicAlbumLabel->setObjectName(QStringLiteral("musicAlbumLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(musicAlbumLabel->sizePolicy().hasHeightForWidth());
        musicAlbumLabel->setSizePolicy(sizePolicy4);

        infoBox->addWidget(musicAlbumLabel);

        musicArtistLabel = new QLabel(centralWidget);
        musicArtistLabel->setObjectName(QStringLiteral("musicArtistLabel"));
        sizePolicy4.setHeightForWidth(musicArtistLabel->sizePolicy().hasHeightForWidth());
        musicArtistLabel->setSizePolicy(sizePolicy4);

        infoBox->addWidget(musicArtistLabel);

        musicYearLabel = new QLabel(centralWidget);
        musicYearLabel->setObjectName(QStringLiteral("musicYearLabel"));
        sizePolicy4.setHeightForWidth(musicYearLabel->sizePolicy().hasHeightForWidth());
        musicYearLabel->setSizePolicy(sizePolicy4);

        infoBox->addWidget(musicYearLabel);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        sizePolicy3.setHeightForWidth(line_3->sizePolicy().hasHeightForWidth());
        line_3->setSizePolicy(sizePolicy3);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line_3);

        soundInfoLabel = new QLabel(centralWidget);
        soundInfoLabel->setObjectName(QStringLiteral("soundInfoLabel"));
        sizePolicy4.setHeightForWidth(soundInfoLabel->sizePolicy().hasHeightForWidth());
        soundInfoLabel->setSizePolicy(sizePolicy4);

        infoBox->addWidget(soundInfoLabel);

        soundNameLabel = new QLabel(centralWidget);
        soundNameLabel->setObjectName(QStringLiteral("soundNameLabel"));
        sizePolicy4.setHeightForWidth(soundNameLabel->sizePolicy().hasHeightForWidth());
        soundNameLabel->setSizePolicy(sizePolicy4);

        infoBox->addWidget(soundNameLabel);

        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        infoBox->addWidget(line_4);

        coverView = new QGraphicsView(centralWidget);
        coverView->setObjectName(QStringLiteral("coverView"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(coverView->sizePolicy().hasHeightForWidth());
        coverView->setSizePolicy(sizePolicy5);
        coverView->setMinimumSize(QSize(300, 300));

        infoBox->addWidget(coverView);


        horizontalLayout_2->addLayout(infoBox);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabMusic = new QWidget();
        tabMusic->setObjectName(QStringLiteral("tabMusic"));
        verticalLayout_3 = new QVBoxLayout(tabMusic);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        toolBox = new QToolBox(tabMusic);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        sizePolicy.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 924, 380));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        scrollArea = new QScrollArea(page);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMaximumSize(QSize(16777215, 16777215));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 904, 360));
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        toolBox->addItem(page, QStringLiteral("Page 1"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 924, 380));
        verticalLayout_4 = new QVBoxLayout(page_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        toolBox->addItem(page_2, QStringLiteral("Page 2"));

        verticalLayout_3->addWidget(toolBox);

        treeWidget = new QTreeWidget(tabMusic);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy6);
        treeWidget->setMaximumSize(QSize(16777215, 350));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        treeWidget->setFont(font);
        treeWidget->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        treeWidget->header()->setVisible(true);
        treeWidget->header()->setCascadingSectionResizes(false);
        treeWidget->header()->setDefaultSectionSize(150);
        treeWidget->header()->setHighlightSections(true);
        treeWidget->header()->setProperty("showSortIndicator", QVariant(true));
        treeWidget->header()->setStretchLastSection(true);

        verticalLayout_3->addWidget(treeWidget);

        tabWidget->addTab(tabMusic, QString());
        tabSound = new QWidget();
        tabSound->setObjectName(QStringLiteral("tabSound"));
        verticalLayout_5 = new QVBoxLayout(tabSound);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        scrollArea_2 = new QScrollArea(tabSound);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 922, 630));
        gridLayout = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_5->addWidget(scrollArea_2);

        tabWidget->addTab(tabSound, QString());

        horizontalLayout_2->addWidget(tabWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 25));
        QSizePolicy sizePolicy7(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(menuBar->sizePolicy().hasHeightForWidth());
        menuBar->setSizePolicy(sizePolicy7);
        menuBar->setMinimumSize(QSize(0, 25));
        menuBar->setBaseSize(QSize(0, 25));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        menuBar->setFont(font1);
        menuBar->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: rgb(102, 102, 102);"));
        menuGM_Help = new QMenu(menuBar);
        menuGM_Help->setObjectName(QStringLiteral("menuGM_Help"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuGM_Help->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuGM_Help->addAction(actionDice);
        menuGM_Help->addAction(actionDatabase);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GM-Companion", Q_NULLPTR));
        actionDice->setText(QApplication::translate("MainWindow", "Dice", Q_NULLPTR));
        actionDatabase->setText(QApplication::translate("MainWindow", "Database", Q_NULLPTR));
        musicLabel->setText(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
        musicPlayButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        musicReplayButton->setText(QApplication::translate("MainWindow", "Replay", Q_NULLPTR));
        musicNextButton->setText(QApplication::translate("MainWindow", "Next", Q_NULLPTR));
        soundLabel->setText(QApplication::translate("MainWindow", "Sound", Q_NULLPTR));
        soundPlayButton->setText(QApplication::translate("MainWindow", "Play", Q_NULLPTR));
        soundReplayButton->setText(QApplication::translate("MainWindow", "Replay", Q_NULLPTR));
        soundNextButton->setText(QApplication::translate("MainWindow", "Next", Q_NULLPTR));
        musicInfoLabel->setText(QApplication::translate("MainWindow", "Music Information", Q_NULLPTR));
        musicTitleLabel->setText(QApplication::translate("MainWindow", "Title", Q_NULLPTR));
        musicAlbumLabel->setText(QApplication::translate("MainWindow", "Album", Q_NULLPTR));
        musicArtistLabel->setText(QApplication::translate("MainWindow", "Artist", Q_NULLPTR));
        musicYearLabel->setText(QApplication::translate("MainWindow", "Year", Q_NULLPTR));
        soundInfoLabel->setText(QApplication::translate("MainWindow", "Sound Information", Q_NULLPTR));
        soundNameLabel->setText(QApplication::translate("MainWindow", "Name", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        tabMusic->setToolTip(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_ACCESSIBILITY
        tabMusic->setAccessibleName(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
#endif // QT_NO_ACCESSIBILITY
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "Page 1", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "Page 2", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("MainWindow", "Year", Q_NULLPTR));
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWindow", "Artist", Q_NULLPTR));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "Album", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Title", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabMusic), QApplication::translate("MainWindow", "Music", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSound), QApplication::translate("MainWindow", "Sound", Q_NULLPTR));
        menuGM_Help->setTitle(QApplication::translate("MainWindow", "GM-Help", Q_NULLPTR));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
