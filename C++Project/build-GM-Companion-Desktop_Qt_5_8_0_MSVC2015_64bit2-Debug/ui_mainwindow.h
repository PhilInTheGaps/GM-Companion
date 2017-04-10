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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDice;
    QAction *actionDatabase;
    QWidget *centralWidget;
    QTreeWidget *treeWidget;
    QMenuBar *menuBar;
    QMenu *menuGM_Help;
    QMenu *menuMusic;
    QMenu *menuSounds;
    QMenu *menuMaps;
    QMenu *menuOptions;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 720);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/ico/resources/icon256.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setTabShape(QTabWidget::Triangular);
        actionDice = new QAction(MainWindow);
        actionDice->setObjectName(QStringLiteral("actionDice"));
        actionDatabase = new QAction(MainWindow);
        actionDatabase->setObjectName(QStringLiteral("actionDatabase"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        treeWidget = new QTreeWidget(centralWidget);
        QIcon icon1;
        icon1.addFile(QStringLiteral("../../../../Google Drive/GM Boys/GM-Companion/Resources/Icons/Music/OSTs/Game of Thrones Season 1.jpg"), QSize(), QIcon::Normal, QIcon::On);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        __qtreewidgetitem->setIcon(1, icon1);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(270, 460, 1011, 221));
        treeWidget->header()->setVisible(true);
        treeWidget->header()->setCascadingSectionResizes(false);
        treeWidget->header()->setDefaultSectionSize(150);
        treeWidget->header()->setHighlightSections(true);
        treeWidget->header()->setProperty("showSortIndicator", QVariant(true));
        treeWidget->header()->setStretchLastSection(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 20));
        menuGM_Help = new QMenu(menuBar);
        menuGM_Help->setObjectName(QStringLiteral("menuGM_Help"));
        menuMusic = new QMenu(menuBar);
        menuMusic->setObjectName(QStringLiteral("menuMusic"));
        menuSounds = new QMenu(menuBar);
        menuSounds->setObjectName(QStringLiteral("menuSounds"));
        menuMaps = new QMenu(menuBar);
        menuMaps->setObjectName(QStringLiteral("menuMaps"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuGM_Help->menuAction());
        menuBar->addAction(menuMusic->menuAction());
        menuBar->addAction(menuSounds->menuAction());
        menuBar->addAction(menuMaps->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuGM_Help->addAction(actionDice);
        menuGM_Help->addAction(actionDatabase);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GM-Companion", Q_NULLPTR));
        actionDice->setText(QApplication::translate("MainWindow", "Dice", Q_NULLPTR));
        actionDatabase->setText(QApplication::translate("MainWindow", "Database", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("MainWindow", "Year", Q_NULLPTR));
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWindow", "Artist", Q_NULLPTR));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "Album", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Title", Q_NULLPTR));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(3, QApplication::translate("MainWindow", "2011", Q_NULLPTR));
        ___qtreewidgetitem1->setText(2, QApplication::translate("MainWindow", "Ramin Djawadi", Q_NULLPTR));
        ___qtreewidgetitem1->setText(1, QApplication::translate("MainWindow", "Game of Thrones Season 1", Q_NULLPTR));
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "Main Theme", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "New Item", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget->topLevelItem(2);
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindow", "New Item", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(3);
        ___qtreewidgetitem4->setText(0, QApplication::translate("MainWindow", "New Item", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem5 = treeWidget->topLevelItem(4);
        ___qtreewidgetitem5->setText(0, QApplication::translate("MainWindow", "New Item", Q_NULLPTR));
        treeWidget->setSortingEnabled(__sortingEnabled);

        menuGM_Help->setTitle(QApplication::translate("MainWindow", "GM-Help", Q_NULLPTR));
        menuMusic->setTitle(QApplication::translate("MainWindow", "Music", Q_NULLPTR));
        menuSounds->setTitle(QApplication::translate("MainWindow", "Sounds", Q_NULLPTR));
        menuMaps->setTitle(QApplication::translate("MainWindow", "Maps", Q_NULLPTR));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
