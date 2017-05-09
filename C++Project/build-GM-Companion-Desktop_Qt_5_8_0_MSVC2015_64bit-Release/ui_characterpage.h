/********************************************************************************
** Form generated from reading UI file 'characterpage.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARACTERPAGE_H
#define UI_CHARACTERPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CharacterPage
{
public:
    QWidget *pageGeneric;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabGeneral_generic;
    QHBoxLayout *horizontalLayout;
    QTableWidget *generalInfoTable_generic;
    QWidget *tabSkills_generic;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTableWidget *skills1Table_generic;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QTableWidget *skills2Table_generic;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QTableWidget *skills3Table_generic;
    QWidget *tabFight_generic;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_4;
    QTableWidget *weaponsTable_generic;
    QLabel *label_5;
    QTableWidget *armorTable_generic;
    QWidget *tabInventory_generic;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_6;
    QTableWidget *inv1Table_generic;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_7;
    QTableWidget *inv2Table_generic;
    QWidget *pageDSA5;
    QVBoxLayout *verticalLayout_8;
    QTabWidget *tabWidget_2;
    QWidget *tabPersDaten_dsa5;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_7;
    QLabel *dsa5Label;
    QSpacerItem *horizontalSpacer;
    QTableWidget *eigenschaftenTable_dsa5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_5;
    QTableWidget *persDaten1Table_dsa5;
    QTableWidget *persDaten2Table_dsa5;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_9;
    QTableWidget *vorteileTable_dsa5;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_8;
    QTableWidget *nachteileTable_dsa5;
    QLabel *label_10;
    QTableWidget *sonderfTable_dsa5;
    QVBoxLayout *verticalLayout_10;
    QTableWidget *apTable_dsa5;
    QTableWidget *allgemeinTable_dsa5;
    QLabel *label_11;
    QTableWidget *schicksalspTable_dsa5;
    QWidget *tabSpielwerte_dsa5;
    QVBoxLayout *verticalLayout_16;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_8;
    QTableWidget *fertigkeiten1Table_dsa5;
    QTableWidget *fertigkeiten2Table_dsa5;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_13;
    QTableWidget *sprachenTable_dsa5;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_14;
    QTableWidget *schriftenTable_dsa5;
    QWidget *tabKampf_dsa5;
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_18;
    QLabel *label_15;
    QTableWidget *ktwTable_dsa5;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_16;
    QTableWidget *lepTable_dsa5;
    QLabel *label_17;
    QTableWidget *kSonderfTable_dsa5;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_21;
    QLabel *label_19;
    QTableWidget *nahkampfwaffenTable_dsa5;
    QLabel *label_20;
    QTableWidget *fernkampfwaffenTable_dsa5;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_18;
    QTableWidget *ruestungenTable_dsa5;
    QLabel *label_21;
    QTableWidget *schildTable_dsa5;
    QWidget *tabBesitz_dsa5;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_22;
    QLabel *label_22;
    QHBoxLayout *horizontalLayout_13;
    QTableWidget *ausruestung1Table_dsa5;
    QTableWidget *ausruestung2Table_dsa5;
    QVBoxLayout *verticalLayout_23;
    QLabel *label_23;
    QTableWidget *geldTable_dsa5;
    QLabel *label_25;
    QTableWidget *tierSonderfTable_dsa5;
    QVBoxLayout *verticalLayout_24;
    QLabel *label_24;
    QTableWidget *tierAllgemeinTable_dsa5;
    QTableWidget *tierAngrifTable_dsa5;
    QLabel *label_27;
    QTableWidget *tierAktionen_dsa5;
    QWidget *tabZauber_dsa5;
    QVBoxLayout *verticalLayout_30;
    QTableWidget *zauberTable_dsa5;
    QHBoxLayout *horizontalLayout_15;
    QVBoxLayout *verticalLayout_28;
    QLabel *label_29;
    QTableWidget *zaubertricksTable_dsa5;
    QLabel *label_30;
    QTableWidget *magiSonderfTable_dsa5;
    QVBoxLayout *verticalLayout_29;
    QTableWidget *leiteigMerkmalTable_dsa5;
    QTableWidget *mTraditionTable_dsa5;
    QWidget *tabLiturgien_dsa5;
    QVBoxLayout *verticalLayout_25;
    QTableWidget *liturgienTable_dsa5;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_27;
    QLabel *label_26;
    QTableWidget *segnungenTable_dsa5;
    QLabel *label_28;
    QTableWidget *kleriSonderfTable_dsa5;
    QVBoxLayout *verticalLayout_26;
    QTableWidget *leiteigAspektTable_dsa5;
    QTableWidget *kTraditionTable_dsa5;

    void setupUi(QStackedWidget *CharacterPage)
    {
        if (CharacterPage->objectName().isEmpty())
            CharacterPage->setObjectName(QStringLiteral("CharacterPage"));
        CharacterPage->resize(1041, 658);
        pageGeneric = new QWidget();
        pageGeneric->setObjectName(QStringLiteral("pageGeneric"));
        verticalLayout = new QVBoxLayout(pageGeneric);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(pageGeneric);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabGeneral_generic = new QWidget();
        tabGeneral_generic->setObjectName(QStringLiteral("tabGeneral_generic"));
        horizontalLayout = new QHBoxLayout(tabGeneral_generic);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        generalInfoTable_generic = new QTableWidget(tabGeneral_generic);
        if (generalInfoTable_generic->columnCount() < 1)
            generalInfoTable_generic->setColumnCount(1);
        if (generalInfoTable_generic->rowCount() < 7)
            generalInfoTable_generic->setRowCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(6, __qtablewidgetitem6);
        generalInfoTable_generic->setObjectName(QStringLiteral("generalInfoTable_generic"));
        generalInfoTable_generic->setColumnCount(1);
        generalInfoTable_generic->horizontalHeader()->setVisible(false);

        horizontalLayout->addWidget(generalInfoTable_generic);

        tabWidget->addTab(tabGeneral_generic, QString());
        tabSkills_generic = new QWidget();
        tabSkills_generic->setObjectName(QStringLiteral("tabSkills_generic"));
        horizontalLayout_2 = new QHBoxLayout(tabSkills_generic);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(tabSkills_generic);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        skills1Table_generic = new QTableWidget(tabSkills_generic);
        if (skills1Table_generic->columnCount() < 2)
            skills1Table_generic->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        skills1Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        skills1Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        skills1Table_generic->setObjectName(QStringLiteral("skills1Table_generic"));
        skills1Table_generic->setColumnCount(2);
        skills1Table_generic->horizontalHeader()->setDefaultSectionSize(120);
        skills1Table_generic->horizontalHeader()->setMinimumSectionSize(80);
        skills1Table_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(skills1Table_generic);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_2 = new QLabel(tabSkills_generic);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_3->addWidget(label_2);

        skills2Table_generic = new QTableWidget(tabSkills_generic);
        if (skills2Table_generic->columnCount() < 2)
            skills2Table_generic->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        skills2Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        skills2Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        skills2Table_generic->setObjectName(QStringLiteral("skills2Table_generic"));
        skills2Table_generic->setColumnCount(2);
        skills2Table_generic->horizontalHeader()->setDefaultSectionSize(120);
        skills2Table_generic->horizontalHeader()->setMinimumSectionSize(80);
        skills2Table_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(skills2Table_generic);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_3 = new QLabel(tabSkills_generic);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_4->addWidget(label_3);

        skills3Table_generic = new QTableWidget(tabSkills_generic);
        if (skills3Table_generic->columnCount() < 2)
            skills3Table_generic->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        skills3Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        skills3Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem12);
        skills3Table_generic->setObjectName(QStringLiteral("skills3Table_generic"));
        skills3Table_generic->setColumnCount(2);
        skills3Table_generic->horizontalHeader()->setDefaultSectionSize(120);
        skills3Table_generic->horizontalHeader()->setMinimumSectionSize(80);
        skills3Table_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_4->addWidget(skills3Table_generic);


        horizontalLayout_2->addLayout(verticalLayout_4);

        tabWidget->addTab(tabSkills_generic, QString());
        tabFight_generic = new QWidget();
        tabFight_generic->setObjectName(QStringLiteral("tabFight_generic"));
        verticalLayout_5 = new QVBoxLayout(tabFight_generic);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_4 = new QLabel(tabFight_generic);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_5->addWidget(label_4);

        weaponsTable_generic = new QTableWidget(tabFight_generic);
        if (weaponsTable_generic->columnCount() < 3)
            weaponsTable_generic->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        weaponsTable_generic->setHorizontalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        weaponsTable_generic->setHorizontalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        weaponsTable_generic->setHorizontalHeaderItem(2, __qtablewidgetitem15);
        weaponsTable_generic->setObjectName(QStringLiteral("weaponsTable_generic"));
        weaponsTable_generic->horizontalHeader()->setDefaultSectionSize(200);
        weaponsTable_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_5->addWidget(weaponsTable_generic);

        label_5 = new QLabel(tabFight_generic);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_5->addWidget(label_5);

        armorTable_generic = new QTableWidget(tabFight_generic);
        if (armorTable_generic->columnCount() < 3)
            armorTable_generic->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        armorTable_generic->setHorizontalHeaderItem(0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        armorTable_generic->setHorizontalHeaderItem(1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        armorTable_generic->setHorizontalHeaderItem(2, __qtablewidgetitem18);
        armorTable_generic->setObjectName(QStringLiteral("armorTable_generic"));
        armorTable_generic->horizontalHeader()->setDefaultSectionSize(200);
        armorTable_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_5->addWidget(armorTable_generic);

        tabWidget->addTab(tabFight_generic, QString());
        tabInventory_generic = new QWidget();
        tabInventory_generic->setObjectName(QStringLiteral("tabInventory_generic"));
        horizontalLayout_3 = new QHBoxLayout(tabInventory_generic);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label_6 = new QLabel(tabInventory_generic);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_6->addWidget(label_6);

        inv1Table_generic = new QTableWidget(tabInventory_generic);
        if (inv1Table_generic->columnCount() < 3)
            inv1Table_generic->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        inv1Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        inv1Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        inv1Table_generic->setHorizontalHeaderItem(2, __qtablewidgetitem21);
        inv1Table_generic->setObjectName(QStringLiteral("inv1Table_generic"));
        inv1Table_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_6->addWidget(inv1Table_generic);


        horizontalLayout_3->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_7 = new QLabel(tabInventory_generic);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_7->addWidget(label_7);

        inv2Table_generic = new QTableWidget(tabInventory_generic);
        if (inv2Table_generic->columnCount() < 3)
            inv2Table_generic->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        inv2Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        inv2Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        inv2Table_generic->setHorizontalHeaderItem(2, __qtablewidgetitem24);
        inv2Table_generic->setObjectName(QStringLiteral("inv2Table_generic"));
        inv2Table_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_7->addWidget(inv2Table_generic);


        horizontalLayout_3->addLayout(verticalLayout_7);

        tabWidget->addTab(tabInventory_generic, QString());

        verticalLayout->addWidget(tabWidget);

        CharacterPage->addWidget(pageGeneric);
        pageDSA5 = new QWidget();
        pageDSA5->setObjectName(QStringLiteral("pageDSA5"));
        verticalLayout_8 = new QVBoxLayout(pageDSA5);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        tabWidget_2 = new QTabWidget(pageDSA5);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabPersDaten_dsa5 = new QWidget();
        tabPersDaten_dsa5->setObjectName(QStringLiteral("tabPersDaten_dsa5"));
        verticalLayout_9 = new QVBoxLayout(tabPersDaten_dsa5);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        dsa5Label = new QLabel(tabPersDaten_dsa5);
        dsa5Label->setObjectName(QStringLiteral("dsa5Label"));
        QFont font;
        font.setPointSize(50);
        font.setBold(true);
        font.setWeight(75);
        dsa5Label->setFont(font);

        horizontalLayout_7->addWidget(dsa5Label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        eigenschaftenTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (eigenschaftenTable_dsa5->columnCount() < 8)
            eigenschaftenTable_dsa5->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(6, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        eigenschaftenTable_dsa5->setHorizontalHeaderItem(7, __qtablewidgetitem32);
        if (eigenschaftenTable_dsa5->rowCount() < 1)
            eigenschaftenTable_dsa5->setRowCount(1);
        eigenschaftenTable_dsa5->setObjectName(QStringLiteral("eigenschaftenTable_dsa5"));
        eigenschaftenTable_dsa5->setMinimumSize(QSize(805, 0));
        eigenschaftenTable_dsa5->setMaximumSize(QSize(805, 70));
        eigenschaftenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        eigenschaftenTable_dsa5->setRowCount(1);
        eigenschaftenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        eigenschaftenTable_dsa5->verticalHeader()->setVisible(false);
        eigenschaftenTable_dsa5->verticalHeader()->setStretchLastSection(true);

        horizontalLayout_7->addWidget(eigenschaftenTable_dsa5);


        verticalLayout_9->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        persDaten1Table_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (persDaten1Table_dsa5->columnCount() < 1)
            persDaten1Table_dsa5->setColumnCount(1);
        if (persDaten1Table_dsa5->rowCount() < 8)
            persDaten1Table_dsa5->setRowCount(8);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        persDaten1Table_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem40);
        persDaten1Table_dsa5->setObjectName(QStringLiteral("persDaten1Table_dsa5"));
        persDaten1Table_dsa5->setMinimumSize(QSize(0, 250));
        persDaten1Table_dsa5->setMaximumSize(QSize(16777215, 250));
        persDaten1Table_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        persDaten1Table_dsa5->setAlternatingRowColors(true);
        persDaten1Table_dsa5->setColumnCount(1);
        persDaten1Table_dsa5->horizontalHeader()->setVisible(false);
        persDaten1Table_dsa5->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_5->addWidget(persDaten1Table_dsa5);

        persDaten2Table_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (persDaten2Table_dsa5->columnCount() < 1)
            persDaten2Table_dsa5->setColumnCount(1);
        if (persDaten2Table_dsa5->rowCount() < 8)
            persDaten2Table_dsa5->setRowCount(8);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem41);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem45);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem46);
        QTableWidgetItem *__qtablewidgetitem47 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem47);
        QTableWidgetItem *__qtablewidgetitem48 = new QTableWidgetItem();
        persDaten2Table_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem48);
        persDaten2Table_dsa5->setObjectName(QStringLiteral("persDaten2Table_dsa5"));
        persDaten2Table_dsa5->setMinimumSize(QSize(0, 250));
        persDaten2Table_dsa5->setMaximumSize(QSize(16777215, 250));
        persDaten2Table_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        persDaten2Table_dsa5->setAlternatingRowColors(true);
        persDaten2Table_dsa5->setColumnCount(1);
        persDaten2Table_dsa5->horizontalHeader()->setVisible(false);
        persDaten2Table_dsa5->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_5->addWidget(persDaten2Table_dsa5);


        verticalLayout_11->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        label_9 = new QLabel(tabPersDaten_dsa5);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout_13->addWidget(label_9);

        vorteileTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (vorteileTable_dsa5->columnCount() < 1)
            vorteileTable_dsa5->setColumnCount(1);
        vorteileTable_dsa5->setObjectName(QStringLiteral("vorteileTable_dsa5"));
        vorteileTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        vorteileTable_dsa5->setAlternatingRowColors(true);
        vorteileTable_dsa5->setColumnCount(1);
        vorteileTable_dsa5->horizontalHeader()->setVisible(false);
        vorteileTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        vorteileTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_13->addWidget(vorteileTable_dsa5);


        horizontalLayout_6->addLayout(verticalLayout_13);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        label_8 = new QLabel(tabPersDaten_dsa5);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_12->addWidget(label_8);

        nachteileTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (nachteileTable_dsa5->columnCount() < 1)
            nachteileTable_dsa5->setColumnCount(1);
        nachteileTable_dsa5->setObjectName(QStringLiteral("nachteileTable_dsa5"));
        nachteileTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        nachteileTable_dsa5->setAlternatingRowColors(true);
        nachteileTable_dsa5->setColumnCount(1);
        nachteileTable_dsa5->horizontalHeader()->setVisible(false);
        nachteileTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        nachteileTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_12->addWidget(nachteileTable_dsa5);


        horizontalLayout_6->addLayout(verticalLayout_12);


        verticalLayout_11->addLayout(horizontalLayout_6);

        label_10 = new QLabel(tabPersDaten_dsa5);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_11->addWidget(label_10);

        sonderfTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (sonderfTable_dsa5->columnCount() < 1)
            sonderfTable_dsa5->setColumnCount(1);
        sonderfTable_dsa5->setObjectName(QStringLiteral("sonderfTable_dsa5"));
        sonderfTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sonderfTable_dsa5->setAlternatingRowColors(true);
        sonderfTable_dsa5->setColumnCount(1);
        sonderfTable_dsa5->horizontalHeader()->setVisible(false);
        sonderfTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        sonderfTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_11->addWidget(sonderfTable_dsa5);


        horizontalLayout_4->addLayout(verticalLayout_11);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        apTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (apTable_dsa5->columnCount() < 4)
            apTable_dsa5->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem49 = new QTableWidgetItem();
        apTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem49);
        QTableWidgetItem *__qtablewidgetitem50 = new QTableWidgetItem();
        apTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem50);
        QTableWidgetItem *__qtablewidgetitem51 = new QTableWidgetItem();
        apTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem51);
        QTableWidgetItem *__qtablewidgetitem52 = new QTableWidgetItem();
        apTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem52);
        if (apTable_dsa5->rowCount() < 1)
            apTable_dsa5->setRowCount(1);
        apTable_dsa5->setObjectName(QStringLiteral("apTable_dsa5"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(apTable_dsa5->sizePolicy().hasHeightForWidth());
        apTable_dsa5->setSizePolicy(sizePolicy);
        apTable_dsa5->setMinimumSize(QSize(0, 0));
        apTable_dsa5->setMaximumSize(QSize(402, 70));
        apTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        apTable_dsa5->setAlternatingRowColors(true);
        apTable_dsa5->setRowCount(1);
        apTable_dsa5->horizontalHeader()->setDefaultSectionSize(90);
        apTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        apTable_dsa5->verticalHeader()->setVisible(false);
        apTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_10->addWidget(apTable_dsa5);

        allgemeinTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (allgemeinTable_dsa5->columnCount() < 4)
            allgemeinTable_dsa5->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem53 = new QTableWidgetItem();
        allgemeinTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem53);
        QTableWidgetItem *__qtablewidgetitem54 = new QTableWidgetItem();
        allgemeinTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem54);
        QTableWidgetItem *__qtablewidgetitem55 = new QTableWidgetItem();
        allgemeinTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem55);
        QTableWidgetItem *__qtablewidgetitem56 = new QTableWidgetItem();
        allgemeinTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem56);
        if (allgemeinTable_dsa5->rowCount() < 8)
            allgemeinTable_dsa5->setRowCount(8);
        QTableWidgetItem *__qtablewidgetitem57 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem57);
        QTableWidgetItem *__qtablewidgetitem58 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem58);
        QTableWidgetItem *__qtablewidgetitem59 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem59);
        QTableWidgetItem *__qtablewidgetitem60 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem60);
        QTableWidgetItem *__qtablewidgetitem61 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem61);
        QTableWidgetItem *__qtablewidgetitem62 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem62);
        QTableWidgetItem *__qtablewidgetitem63 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem63);
        QTableWidgetItem *__qtablewidgetitem64 = new QTableWidgetItem();
        allgemeinTable_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem64);
        allgemeinTable_dsa5->setObjectName(QStringLiteral("allgemeinTable_dsa5"));
        allgemeinTable_dsa5->setMaximumSize(QSize(402, 16777215));
        allgemeinTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        allgemeinTable_dsa5->setAlternatingRowColors(true);
        allgemeinTable_dsa5->horizontalHeader()->setDefaultSectionSize(74);

        verticalLayout_10->addWidget(allgemeinTable_dsa5);

        label_11 = new QLabel(tabPersDaten_dsa5);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_10->addWidget(label_11);

        schicksalspTable_dsa5 = new QTableWidget(tabPersDaten_dsa5);
        if (schicksalspTable_dsa5->columnCount() < 4)
            schicksalspTable_dsa5->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem65 = new QTableWidgetItem();
        schicksalspTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem65);
        QTableWidgetItem *__qtablewidgetitem66 = new QTableWidgetItem();
        schicksalspTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem66);
        QTableWidgetItem *__qtablewidgetitem67 = new QTableWidgetItem();
        schicksalspTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem67);
        QTableWidgetItem *__qtablewidgetitem68 = new QTableWidgetItem();
        schicksalspTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem68);
        if (schicksalspTable_dsa5->rowCount() < 1)
            schicksalspTable_dsa5->setRowCount(1);
        schicksalspTable_dsa5->setObjectName(QStringLiteral("schicksalspTable_dsa5"));
        schicksalspTable_dsa5->setMaximumSize(QSize(402, 70));
        schicksalspTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        schicksalspTable_dsa5->setAlternatingRowColors(true);
        schicksalspTable_dsa5->setRowCount(1);
        schicksalspTable_dsa5->horizontalHeader()->setDefaultSectionSize(90);
        schicksalspTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        schicksalspTable_dsa5->verticalHeader()->setVisible(false);
        schicksalspTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_10->addWidget(schicksalspTable_dsa5);


        horizontalLayout_4->addLayout(verticalLayout_10);


        verticalLayout_9->addLayout(horizontalLayout_4);

        tabWidget_2->addTab(tabPersDaten_dsa5, QString());
        tabSpielwerte_dsa5 = new QWidget();
        tabSpielwerte_dsa5->setObjectName(QStringLiteral("tabSpielwerte_dsa5"));
        verticalLayout_16 = new QVBoxLayout(tabSpielwerte_dsa5);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        label_12 = new QLabel(tabSpielwerte_dsa5);
        label_12->setObjectName(QStringLiteral("label_12"));

        verticalLayout_16->addWidget(label_12);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        fertigkeiten1Table_dsa5 = new QTableWidget(tabSpielwerte_dsa5);
        if (fertigkeiten1Table_dsa5->columnCount() < 6)
            fertigkeiten1Table_dsa5->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem69 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem69);
        QTableWidgetItem *__qtablewidgetitem70 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem70);
        QTableWidgetItem *__qtablewidgetitem71 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem71);
        QTableWidgetItem *__qtablewidgetitem72 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem72);
        QTableWidgetItem *__qtablewidgetitem73 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem73);
        QTableWidgetItem *__qtablewidgetitem74 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem74);
        if (fertigkeiten1Table_dsa5->rowCount() < 35)
            fertigkeiten1Table_dsa5->setRowCount(35);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem75 = new QTableWidgetItem();
        __qtablewidgetitem75->setFont(font1);
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem75);
        QTableWidgetItem *__qtablewidgetitem76 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem76);
        QTableWidgetItem *__qtablewidgetitem77 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem77);
        QTableWidgetItem *__qtablewidgetitem78 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem78);
        QTableWidgetItem *__qtablewidgetitem79 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem79);
        QTableWidgetItem *__qtablewidgetitem80 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem80);
        QTableWidgetItem *__qtablewidgetitem81 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem81);
        QTableWidgetItem *__qtablewidgetitem82 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem82);
        QTableWidgetItem *__qtablewidgetitem83 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(8, __qtablewidgetitem83);
        QTableWidgetItem *__qtablewidgetitem84 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(9, __qtablewidgetitem84);
        QTableWidgetItem *__qtablewidgetitem85 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(10, __qtablewidgetitem85);
        QTableWidgetItem *__qtablewidgetitem86 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(11, __qtablewidgetitem86);
        QTableWidgetItem *__qtablewidgetitem87 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(12, __qtablewidgetitem87);
        QTableWidgetItem *__qtablewidgetitem88 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(13, __qtablewidgetitem88);
        QTableWidgetItem *__qtablewidgetitem89 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(14, __qtablewidgetitem89);
        QTableWidgetItem *__qtablewidgetitem90 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(15, __qtablewidgetitem90);
        QTableWidgetItem *__qtablewidgetitem91 = new QTableWidgetItem();
        __qtablewidgetitem91->setFont(font1);
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(16, __qtablewidgetitem91);
        QTableWidgetItem *__qtablewidgetitem92 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(17, __qtablewidgetitem92);
        QTableWidgetItem *__qtablewidgetitem93 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(18, __qtablewidgetitem93);
        QTableWidgetItem *__qtablewidgetitem94 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(19, __qtablewidgetitem94);
        QTableWidgetItem *__qtablewidgetitem95 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(20, __qtablewidgetitem95);
        QTableWidgetItem *__qtablewidgetitem96 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(21, __qtablewidgetitem96);
        QTableWidgetItem *__qtablewidgetitem97 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(22, __qtablewidgetitem97);
        QTableWidgetItem *__qtablewidgetitem98 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(23, __qtablewidgetitem98);
        QTableWidgetItem *__qtablewidgetitem99 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(24, __qtablewidgetitem99);
        QTableWidgetItem *__qtablewidgetitem100 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(25, __qtablewidgetitem100);
        QTableWidgetItem *__qtablewidgetitem101 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(26, __qtablewidgetitem101);
        QTableWidgetItem *__qtablewidgetitem102 = new QTableWidgetItem();
        __qtablewidgetitem102->setFont(font1);
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(27, __qtablewidgetitem102);
        QTableWidgetItem *__qtablewidgetitem103 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(28, __qtablewidgetitem103);
        QTableWidgetItem *__qtablewidgetitem104 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(29, __qtablewidgetitem104);
        QTableWidgetItem *__qtablewidgetitem105 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(30, __qtablewidgetitem105);
        QTableWidgetItem *__qtablewidgetitem106 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(31, __qtablewidgetitem106);
        QTableWidgetItem *__qtablewidgetitem107 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(32, __qtablewidgetitem107);
        QTableWidgetItem *__qtablewidgetitem108 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(33, __qtablewidgetitem108);
        QTableWidgetItem *__qtablewidgetitem109 = new QTableWidgetItem();
        fertigkeiten1Table_dsa5->setVerticalHeaderItem(34, __qtablewidgetitem109);
        fertigkeiten1Table_dsa5->setObjectName(QStringLiteral("fertigkeiten1Table_dsa5"));
        fertigkeiten1Table_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fertigkeiten1Table_dsa5->setAlternatingRowColors(true);
        fertigkeiten1Table_dsa5->horizontalHeader()->setDefaultSectionSize(80);
        fertigkeiten1Table_dsa5->horizontalHeader()->setStretchLastSection(true);
        fertigkeiten1Table_dsa5->verticalHeader()->setDefaultSectionSize(20);
        fertigkeiten1Table_dsa5->verticalHeader()->setStretchLastSection(false);

        horizontalLayout_8->addWidget(fertigkeiten1Table_dsa5);

        fertigkeiten2Table_dsa5 = new QTableWidget(tabSpielwerte_dsa5);
        if (fertigkeiten2Table_dsa5->columnCount() < 6)
            fertigkeiten2Table_dsa5->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem110 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem110);
        QTableWidgetItem *__qtablewidgetitem111 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem111);
        QTableWidgetItem *__qtablewidgetitem112 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem112);
        QTableWidgetItem *__qtablewidgetitem113 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem113);
        QTableWidgetItem *__qtablewidgetitem114 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem114);
        QTableWidgetItem *__qtablewidgetitem115 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem115);
        if (fertigkeiten2Table_dsa5->rowCount() < 32)
            fertigkeiten2Table_dsa5->setRowCount(32);
        QTableWidgetItem *__qtablewidgetitem116 = new QTableWidgetItem();
        __qtablewidgetitem116->setFont(font1);
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem116);
        QTableWidgetItem *__qtablewidgetitem117 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem117);
        QTableWidgetItem *__qtablewidgetitem118 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem118);
        QTableWidgetItem *__qtablewidgetitem119 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem119);
        QTableWidgetItem *__qtablewidgetitem120 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem120);
        QTableWidgetItem *__qtablewidgetitem121 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem121);
        QTableWidgetItem *__qtablewidgetitem122 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem122);
        QTableWidgetItem *__qtablewidgetitem123 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem123);
        QTableWidgetItem *__qtablewidgetitem124 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(8, __qtablewidgetitem124);
        QTableWidgetItem *__qtablewidgetitem125 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(9, __qtablewidgetitem125);
        QTableWidgetItem *__qtablewidgetitem126 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(10, __qtablewidgetitem126);
        QTableWidgetItem *__qtablewidgetitem127 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(11, __qtablewidgetitem127);
        QTableWidgetItem *__qtablewidgetitem128 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(12, __qtablewidgetitem128);
        QTableWidgetItem *__qtablewidgetitem129 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(13, __qtablewidgetitem129);
        QTableWidgetItem *__qtablewidgetitem130 = new QTableWidgetItem();
        __qtablewidgetitem130->setFont(font1);
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(14, __qtablewidgetitem130);
        QTableWidgetItem *__qtablewidgetitem131 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(15, __qtablewidgetitem131);
        QTableWidgetItem *__qtablewidgetitem132 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(16, __qtablewidgetitem132);
        QTableWidgetItem *__qtablewidgetitem133 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(17, __qtablewidgetitem133);
        QTableWidgetItem *__qtablewidgetitem134 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(18, __qtablewidgetitem134);
        QTableWidgetItem *__qtablewidgetitem135 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(19, __qtablewidgetitem135);
        QTableWidgetItem *__qtablewidgetitem136 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(20, __qtablewidgetitem136);
        QTableWidgetItem *__qtablewidgetitem137 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(21, __qtablewidgetitem137);
        QTableWidgetItem *__qtablewidgetitem138 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(22, __qtablewidgetitem138);
        QTableWidgetItem *__qtablewidgetitem139 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(23, __qtablewidgetitem139);
        QTableWidgetItem *__qtablewidgetitem140 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(24, __qtablewidgetitem140);
        QTableWidgetItem *__qtablewidgetitem141 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(25, __qtablewidgetitem141);
        QTableWidgetItem *__qtablewidgetitem142 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(26, __qtablewidgetitem142);
        QTableWidgetItem *__qtablewidgetitem143 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(27, __qtablewidgetitem143);
        QTableWidgetItem *__qtablewidgetitem144 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(28, __qtablewidgetitem144);
        QTableWidgetItem *__qtablewidgetitem145 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(29, __qtablewidgetitem145);
        QTableWidgetItem *__qtablewidgetitem146 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(30, __qtablewidgetitem146);
        QTableWidgetItem *__qtablewidgetitem147 = new QTableWidgetItem();
        fertigkeiten2Table_dsa5->setVerticalHeaderItem(31, __qtablewidgetitem147);
        fertigkeiten2Table_dsa5->setObjectName(QStringLiteral("fertigkeiten2Table_dsa5"));
        fertigkeiten2Table_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fertigkeiten2Table_dsa5->setAlternatingRowColors(true);
        fertigkeiten2Table_dsa5->horizontalHeader()->setDefaultSectionSize(80);
        fertigkeiten2Table_dsa5->horizontalHeader()->setStretchLastSection(true);
        fertigkeiten2Table_dsa5->verticalHeader()->setDefaultSectionSize(20);
        fertigkeiten2Table_dsa5->verticalHeader()->setStretchLastSection(false);

        horizontalLayout_8->addWidget(fertigkeiten2Table_dsa5);


        verticalLayout_16->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        label_13 = new QLabel(tabSpielwerte_dsa5);
        label_13->setObjectName(QStringLiteral("label_13"));

        verticalLayout_15->addWidget(label_13);

        sprachenTable_dsa5 = new QTableWidget(tabSpielwerte_dsa5);
        if (sprachenTable_dsa5->columnCount() < 1)
            sprachenTable_dsa5->setColumnCount(1);
        sprachenTable_dsa5->setObjectName(QStringLiteral("sprachenTable_dsa5"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sprachenTable_dsa5->sizePolicy().hasHeightForWidth());
        sprachenTable_dsa5->setSizePolicy(sizePolicy1);
        sprachenTable_dsa5->setMaximumSize(QSize(16777215, 100));
        sprachenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sprachenTable_dsa5->setAlternatingRowColors(true);
        sprachenTable_dsa5->setColumnCount(1);
        sprachenTable_dsa5->horizontalHeader()->setVisible(false);
        sprachenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        sprachenTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_15->addWidget(sprachenTable_dsa5);


        horizontalLayout_9->addLayout(verticalLayout_15);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        label_14 = new QLabel(tabSpielwerte_dsa5);
        label_14->setObjectName(QStringLiteral("label_14"));

        verticalLayout_14->addWidget(label_14);

        schriftenTable_dsa5 = new QTableWidget(tabSpielwerte_dsa5);
        if (schriftenTable_dsa5->columnCount() < 1)
            schriftenTable_dsa5->setColumnCount(1);
        schriftenTable_dsa5->setObjectName(QStringLiteral("schriftenTable_dsa5"));
        sizePolicy1.setHeightForWidth(schriftenTable_dsa5->sizePolicy().hasHeightForWidth());
        schriftenTable_dsa5->setSizePolicy(sizePolicy1);
        schriftenTable_dsa5->setMaximumSize(QSize(16777215, 100));
        schriftenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        schriftenTable_dsa5->setAlternatingRowColors(true);
        schriftenTable_dsa5->setColumnCount(1);
        schriftenTable_dsa5->horizontalHeader()->setVisible(false);
        schriftenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        schriftenTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_14->addWidget(schriftenTable_dsa5);


        horizontalLayout_9->addLayout(verticalLayout_14);


        verticalLayout_16->addLayout(horizontalLayout_9);

        tabWidget_2->addTab(tabSpielwerte_dsa5, QString());
        tabKampf_dsa5 = new QWidget();
        tabKampf_dsa5->setObjectName(QStringLiteral("tabKampf_dsa5"));
        verticalLayout_17 = new QVBoxLayout(tabKampf_dsa5);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        label_15 = new QLabel(tabKampf_dsa5);
        label_15->setObjectName(QStringLiteral("label_15"));

        verticalLayout_18->addWidget(label_15);

        ktwTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        if (ktwTable_dsa5->columnCount() < 5)
            ktwTable_dsa5->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem148 = new QTableWidgetItem();
        ktwTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem148);
        QTableWidgetItem *__qtablewidgetitem149 = new QTableWidgetItem();
        ktwTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem149);
        QTableWidgetItem *__qtablewidgetitem150 = new QTableWidgetItem();
        ktwTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem150);
        QTableWidgetItem *__qtablewidgetitem151 = new QTableWidgetItem();
        ktwTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem151);
        QTableWidgetItem *__qtablewidgetitem152 = new QTableWidgetItem();
        ktwTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem152);
        if (ktwTable_dsa5->rowCount() < 13)
            ktwTable_dsa5->setRowCount(13);
        QTableWidgetItem *__qtablewidgetitem153 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem153);
        QTableWidgetItem *__qtablewidgetitem154 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem154);
        QTableWidgetItem *__qtablewidgetitem155 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem155);
        QTableWidgetItem *__qtablewidgetitem156 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem156);
        QTableWidgetItem *__qtablewidgetitem157 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem157);
        QTableWidgetItem *__qtablewidgetitem158 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem158);
        QTableWidgetItem *__qtablewidgetitem159 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem159);
        QTableWidgetItem *__qtablewidgetitem160 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem160);
        QTableWidgetItem *__qtablewidgetitem161 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(8, __qtablewidgetitem161);
        QTableWidgetItem *__qtablewidgetitem162 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(9, __qtablewidgetitem162);
        QTableWidgetItem *__qtablewidgetitem163 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(10, __qtablewidgetitem163);
        QTableWidgetItem *__qtablewidgetitem164 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(11, __qtablewidgetitem164);
        QTableWidgetItem *__qtablewidgetitem165 = new QTableWidgetItem();
        ktwTable_dsa5->setVerticalHeaderItem(12, __qtablewidgetitem165);
        ktwTable_dsa5->setObjectName(QStringLiteral("ktwTable_dsa5"));
        ktwTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout_18->addWidget(ktwTable_dsa5);


        horizontalLayout_10->addLayout(verticalLayout_18);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        label_16 = new QLabel(tabKampf_dsa5);
        label_16->setObjectName(QStringLiteral("label_16"));

        verticalLayout_19->addWidget(label_16);

        lepTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        if (lepTable_dsa5->columnCount() < 2)
            lepTable_dsa5->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem166 = new QTableWidgetItem();
        lepTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem166);
        QTableWidgetItem *__qtablewidgetitem167 = new QTableWidgetItem();
        lepTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem167);
        if (lepTable_dsa5->rowCount() < 1)
            lepTable_dsa5->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem168 = new QTableWidgetItem();
        lepTable_dsa5->setItem(0, 0, __qtablewidgetitem168);
        QTableWidgetItem *__qtablewidgetitem169 = new QTableWidgetItem();
        lepTable_dsa5->setItem(0, 1, __qtablewidgetitem169);
        lepTable_dsa5->setObjectName(QStringLiteral("lepTable_dsa5"));
        lepTable_dsa5->setMaximumSize(QSize(16777215, 100));
        lepTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lepTable_dsa5->setRowCount(1);
        lepTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        lepTable_dsa5->verticalHeader()->setVisible(false);
        lepTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_19->addWidget(lepTable_dsa5);

        label_17 = new QLabel(tabKampf_dsa5);
        label_17->setObjectName(QStringLiteral("label_17"));

        verticalLayout_19->addWidget(label_17);

        kSonderfTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        kSonderfTable_dsa5->setObjectName(QStringLiteral("kSonderfTable_dsa5"));
        kSonderfTable_dsa5->setMinimumSize(QSize(0, 200));
        kSonderfTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        kSonderfTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        kSonderfTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_19->addWidget(kSonderfTable_dsa5);


        horizontalLayout_10->addLayout(verticalLayout_19);


        verticalLayout_17->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        label_19 = new QLabel(tabKampf_dsa5);
        label_19->setObjectName(QStringLiteral("label_19"));

        verticalLayout_21->addWidget(label_19);

        nahkampfwaffenTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        if (nahkampfwaffenTable_dsa5->columnCount() < 9)
            nahkampfwaffenTable_dsa5->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem170 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem170);
        QTableWidgetItem *__qtablewidgetitem171 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem171);
        QTableWidgetItem *__qtablewidgetitem172 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem172);
        QTableWidgetItem *__qtablewidgetitem173 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem173);
        QTableWidgetItem *__qtablewidgetitem174 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem174);
        QTableWidgetItem *__qtablewidgetitem175 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem175);
        QTableWidgetItem *__qtablewidgetitem176 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(6, __qtablewidgetitem176);
        QTableWidgetItem *__qtablewidgetitem177 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(7, __qtablewidgetitem177);
        QTableWidgetItem *__qtablewidgetitem178 = new QTableWidgetItem();
        nahkampfwaffenTable_dsa5->setHorizontalHeaderItem(8, __qtablewidgetitem178);
        nahkampfwaffenTable_dsa5->setObjectName(QStringLiteral("nahkampfwaffenTable_dsa5"));
        nahkampfwaffenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        nahkampfwaffenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        nahkampfwaffenTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_21->addWidget(nahkampfwaffenTable_dsa5);

        label_20 = new QLabel(tabKampf_dsa5);
        label_20->setObjectName(QStringLiteral("label_20"));

        verticalLayout_21->addWidget(label_20);

        fernkampfwaffenTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        if (fernkampfwaffenTable_dsa5->columnCount() < 8)
            fernkampfwaffenTable_dsa5->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem179 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem179);
        QTableWidgetItem *__qtablewidgetitem180 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem180);
        QTableWidgetItem *__qtablewidgetitem181 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem181);
        QTableWidgetItem *__qtablewidgetitem182 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem182);
        QTableWidgetItem *__qtablewidgetitem183 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem183);
        QTableWidgetItem *__qtablewidgetitem184 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem184);
        QTableWidgetItem *__qtablewidgetitem185 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(6, __qtablewidgetitem185);
        QTableWidgetItem *__qtablewidgetitem186 = new QTableWidgetItem();
        fernkampfwaffenTable_dsa5->setHorizontalHeaderItem(7, __qtablewidgetitem186);
        fernkampfwaffenTable_dsa5->setObjectName(QStringLiteral("fernkampfwaffenTable_dsa5"));
        fernkampfwaffenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fernkampfwaffenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        fernkampfwaffenTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_21->addWidget(fernkampfwaffenTable_dsa5);


        horizontalLayout_11->addLayout(verticalLayout_21);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        label_18 = new QLabel(tabKampf_dsa5);
        label_18->setObjectName(QStringLiteral("label_18"));

        verticalLayout_20->addWidget(label_18);

        ruestungenTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        if (ruestungenTable_dsa5->columnCount() < 6)
            ruestungenTable_dsa5->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem187 = new QTableWidgetItem();
        ruestungenTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem187);
        QTableWidgetItem *__qtablewidgetitem188 = new QTableWidgetItem();
        ruestungenTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem188);
        QTableWidgetItem *__qtablewidgetitem189 = new QTableWidgetItem();
        ruestungenTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem189);
        QTableWidgetItem *__qtablewidgetitem190 = new QTableWidgetItem();
        ruestungenTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem190);
        QTableWidgetItem *__qtablewidgetitem191 = new QTableWidgetItem();
        ruestungenTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem191);
        QTableWidgetItem *__qtablewidgetitem192 = new QTableWidgetItem();
        ruestungenTable_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem192);
        ruestungenTable_dsa5->setObjectName(QStringLiteral("ruestungenTable_dsa5"));
        ruestungenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ruestungenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        ruestungenTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_20->addWidget(ruestungenTable_dsa5);

        label_21 = new QLabel(tabKampf_dsa5);
        label_21->setObjectName(QStringLiteral("label_21"));

        verticalLayout_20->addWidget(label_21);

        schildTable_dsa5 = new QTableWidget(tabKampf_dsa5);
        if (schildTable_dsa5->columnCount() < 4)
            schildTable_dsa5->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem193 = new QTableWidgetItem();
        schildTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem193);
        QTableWidgetItem *__qtablewidgetitem194 = new QTableWidgetItem();
        schildTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem194);
        QTableWidgetItem *__qtablewidgetitem195 = new QTableWidgetItem();
        schildTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem195);
        QTableWidgetItem *__qtablewidgetitem196 = new QTableWidgetItem();
        schildTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem196);
        schildTable_dsa5->setObjectName(QStringLiteral("schildTable_dsa5"));
        schildTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        schildTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        schildTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_20->addWidget(schildTable_dsa5);


        horizontalLayout_11->addLayout(verticalLayout_20);


        verticalLayout_17->addLayout(horizontalLayout_11);

        tabWidget_2->addTab(tabKampf_dsa5, QString());
        tabBesitz_dsa5 = new QWidget();
        tabBesitz_dsa5->setObjectName(QStringLiteral("tabBesitz_dsa5"));
        horizontalLayout_12 = new QHBoxLayout(tabBesitz_dsa5);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        label_22 = new QLabel(tabBesitz_dsa5);
        label_22->setObjectName(QStringLiteral("label_22"));

        verticalLayout_22->addWidget(label_22);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        ausruestung1Table_dsa5 = new QTableWidget(tabBesitz_dsa5);
        if (ausruestung1Table_dsa5->columnCount() < 3)
            ausruestung1Table_dsa5->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem197 = new QTableWidgetItem();
        ausruestung1Table_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem197);
        QTableWidgetItem *__qtablewidgetitem198 = new QTableWidgetItem();
        ausruestung1Table_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem198);
        QTableWidgetItem *__qtablewidgetitem199 = new QTableWidgetItem();
        ausruestung1Table_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem199);
        ausruestung1Table_dsa5->setObjectName(QStringLiteral("ausruestung1Table_dsa5"));
        ausruestung1Table_dsa5->setMinimumSize(QSize(300, 0));
        ausruestung1Table_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ausruestung1Table_dsa5->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_13->addWidget(ausruestung1Table_dsa5);

        ausruestung2Table_dsa5 = new QTableWidget(tabBesitz_dsa5);
        if (ausruestung2Table_dsa5->columnCount() < 3)
            ausruestung2Table_dsa5->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem200 = new QTableWidgetItem();
        ausruestung2Table_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem200);
        QTableWidgetItem *__qtablewidgetitem201 = new QTableWidgetItem();
        ausruestung2Table_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem201);
        QTableWidgetItem *__qtablewidgetitem202 = new QTableWidgetItem();
        ausruestung2Table_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem202);
        ausruestung2Table_dsa5->setObjectName(QStringLiteral("ausruestung2Table_dsa5"));
        ausruestung2Table_dsa5->setMinimumSize(QSize(300, 0));
        ausruestung2Table_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ausruestung2Table_dsa5->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_13->addWidget(ausruestung2Table_dsa5);


        verticalLayout_22->addLayout(horizontalLayout_13);


        horizontalLayout_12->addLayout(verticalLayout_22);

        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        label_23 = new QLabel(tabBesitz_dsa5);
        label_23->setObjectName(QStringLiteral("label_23"));

        verticalLayout_23->addWidget(label_23);

        geldTable_dsa5 = new QTableWidget(tabBesitz_dsa5);
        if (geldTable_dsa5->columnCount() < 1)
            geldTable_dsa5->setColumnCount(1);
        if (geldTable_dsa5->rowCount() < 7)
            geldTable_dsa5->setRowCount(7);
        QTableWidgetItem *__qtablewidgetitem203 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem203);
        QTableWidgetItem *__qtablewidgetitem204 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem204);
        QTableWidgetItem *__qtablewidgetitem205 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem205);
        QTableWidgetItem *__qtablewidgetitem206 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem206);
        QTableWidgetItem *__qtablewidgetitem207 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem207);
        QTableWidgetItem *__qtablewidgetitem208 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem208);
        QTableWidgetItem *__qtablewidgetitem209 = new QTableWidgetItem();
        geldTable_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem209);
        geldTable_dsa5->setObjectName(QStringLiteral("geldTable_dsa5"));
        geldTable_dsa5->setMinimumSize(QSize(0, 300));
        geldTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        geldTable_dsa5->setColumnCount(1);
        geldTable_dsa5->horizontalHeader()->setVisible(false);
        geldTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        geldTable_dsa5->verticalHeader()->setDefaultSectionSize(40);

        verticalLayout_23->addWidget(geldTable_dsa5);

        label_25 = new QLabel(tabBesitz_dsa5);
        label_25->setObjectName(QStringLiteral("label_25"));

        verticalLayout_23->addWidget(label_25);

        tierSonderfTable_dsa5 = new QTableWidget(tabBesitz_dsa5);
        tierSonderfTable_dsa5->setObjectName(QStringLiteral("tierSonderfTable_dsa5"));
        tierSonderfTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tierSonderfTable_dsa5->horizontalHeader()->setVisible(false);
        tierSonderfTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        tierSonderfTable_dsa5->verticalHeader()->setHighlightSections(false);

        verticalLayout_23->addWidget(tierSonderfTable_dsa5);


        horizontalLayout_12->addLayout(verticalLayout_23);

        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        label_24 = new QLabel(tabBesitz_dsa5);
        label_24->setObjectName(QStringLiteral("label_24"));

        verticalLayout_24->addWidget(label_24);

        tierAllgemeinTable_dsa5 = new QTableWidget(tabBesitz_dsa5);
        if (tierAllgemeinTable_dsa5->columnCount() < 1)
            tierAllgemeinTable_dsa5->setColumnCount(1);
        if (tierAllgemeinTable_dsa5->rowCount() < 18)
            tierAllgemeinTable_dsa5->setRowCount(18);
        QTableWidgetItem *__qtablewidgetitem210 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(0, __qtablewidgetitem210);
        QTableWidgetItem *__qtablewidgetitem211 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(1, __qtablewidgetitem211);
        QTableWidgetItem *__qtablewidgetitem212 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(2, __qtablewidgetitem212);
        QTableWidgetItem *__qtablewidgetitem213 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(3, __qtablewidgetitem213);
        QTableWidgetItem *__qtablewidgetitem214 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(4, __qtablewidgetitem214);
        QTableWidgetItem *__qtablewidgetitem215 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(5, __qtablewidgetitem215);
        QTableWidgetItem *__qtablewidgetitem216 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(6, __qtablewidgetitem216);
        QTableWidgetItem *__qtablewidgetitem217 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(7, __qtablewidgetitem217);
        QTableWidgetItem *__qtablewidgetitem218 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(8, __qtablewidgetitem218);
        QTableWidgetItem *__qtablewidgetitem219 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(9, __qtablewidgetitem219);
        QTableWidgetItem *__qtablewidgetitem220 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(10, __qtablewidgetitem220);
        QTableWidgetItem *__qtablewidgetitem221 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(11, __qtablewidgetitem221);
        QTableWidgetItem *__qtablewidgetitem222 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(12, __qtablewidgetitem222);
        QTableWidgetItem *__qtablewidgetitem223 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(13, __qtablewidgetitem223);
        QTableWidgetItem *__qtablewidgetitem224 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(14, __qtablewidgetitem224);
        QTableWidgetItem *__qtablewidgetitem225 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(15, __qtablewidgetitem225);
        QTableWidgetItem *__qtablewidgetitem226 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(16, __qtablewidgetitem226);
        QTableWidgetItem *__qtablewidgetitem227 = new QTableWidgetItem();
        tierAllgemeinTable_dsa5->setVerticalHeaderItem(17, __qtablewidgetitem227);
        tierAllgemeinTable_dsa5->setObjectName(QStringLiteral("tierAllgemeinTable_dsa5"));
        tierAllgemeinTable_dsa5->setMinimumSize(QSize(0, 300));
        tierAllgemeinTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tierAllgemeinTable_dsa5->setColumnCount(1);
        tierAllgemeinTable_dsa5->horizontalHeader()->setVisible(false);
        tierAllgemeinTable_dsa5->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_24->addWidget(tierAllgemeinTable_dsa5);

        tierAngrifTable_dsa5 = new QTableWidget(tabBesitz_dsa5);
        if (tierAngrifTable_dsa5->columnCount() < 5)
            tierAngrifTable_dsa5->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem228 = new QTableWidgetItem();
        tierAngrifTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem228);
        QTableWidgetItem *__qtablewidgetitem229 = new QTableWidgetItem();
        tierAngrifTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem229);
        QTableWidgetItem *__qtablewidgetitem230 = new QTableWidgetItem();
        tierAngrifTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem230);
        QTableWidgetItem *__qtablewidgetitem231 = new QTableWidgetItem();
        tierAngrifTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem231);
        QTableWidgetItem *__qtablewidgetitem232 = new QTableWidgetItem();
        tierAngrifTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem232);
        tierAngrifTable_dsa5->setObjectName(QStringLiteral("tierAngrifTable_dsa5"));
        tierAngrifTable_dsa5->setMaximumSize(QSize(16777215, 150));
        tierAngrifTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tierAngrifTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        tierAngrifTable_dsa5->verticalHeader()->setVisible(false);
        tierAngrifTable_dsa5->verticalHeader()->setDefaultSectionSize(20);

        verticalLayout_24->addWidget(tierAngrifTable_dsa5);

        label_27 = new QLabel(tabBesitz_dsa5);
        label_27->setObjectName(QStringLiteral("label_27"));

        verticalLayout_24->addWidget(label_27);

        tierAktionen_dsa5 = new QTableWidget(tabBesitz_dsa5);
        tierAktionen_dsa5->setObjectName(QStringLiteral("tierAktionen_dsa5"));
        tierAktionen_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tierAktionen_dsa5->horizontalHeader()->setVisible(false);
        tierAktionen_dsa5->horizontalHeader()->setStretchLastSection(true);
        tierAktionen_dsa5->verticalHeader()->setHighlightSections(false);

        verticalLayout_24->addWidget(tierAktionen_dsa5);


        horizontalLayout_12->addLayout(verticalLayout_24);

        tabWidget_2->addTab(tabBesitz_dsa5, QString());
        tabZauber_dsa5 = new QWidget();
        tabZauber_dsa5->setObjectName(QStringLiteral("tabZauber_dsa5"));
        verticalLayout_30 = new QVBoxLayout(tabZauber_dsa5);
        verticalLayout_30->setObjectName(QStringLiteral("verticalLayout_30"));
        zauberTable_dsa5 = new QTableWidget(tabZauber_dsa5);
        if (zauberTable_dsa5->columnCount() < 10)
            zauberTable_dsa5->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem233 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem233);
        QTableWidgetItem *__qtablewidgetitem234 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem234);
        QTableWidgetItem *__qtablewidgetitem235 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem235);
        QTableWidgetItem *__qtablewidgetitem236 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem236);
        QTableWidgetItem *__qtablewidgetitem237 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem237);
        QTableWidgetItem *__qtablewidgetitem238 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem238);
        QTableWidgetItem *__qtablewidgetitem239 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(6, __qtablewidgetitem239);
        QTableWidgetItem *__qtablewidgetitem240 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(7, __qtablewidgetitem240);
        QTableWidgetItem *__qtablewidgetitem241 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(8, __qtablewidgetitem241);
        QTableWidgetItem *__qtablewidgetitem242 = new QTableWidgetItem();
        zauberTable_dsa5->setHorizontalHeaderItem(9, __qtablewidgetitem242);
        zauberTable_dsa5->setObjectName(QStringLiteral("zauberTable_dsa5"));
        zauberTable_dsa5->setMinimumSize(QSize(0, 300));
        zauberTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        zauberTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        zauberTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_30->addWidget(zauberTable_dsa5);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setObjectName(QStringLiteral("verticalLayout_28"));
        label_29 = new QLabel(tabZauber_dsa5);
        label_29->setObjectName(QStringLiteral("label_29"));

        verticalLayout_28->addWidget(label_29);

        zaubertricksTable_dsa5 = new QTableWidget(tabZauber_dsa5);
        if (zaubertricksTable_dsa5->columnCount() < 1)
            zaubertricksTable_dsa5->setColumnCount(1);
        zaubertricksTable_dsa5->setObjectName(QStringLiteral("zaubertricksTable_dsa5"));
        zaubertricksTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        zaubertricksTable_dsa5->setColumnCount(1);
        zaubertricksTable_dsa5->horizontalHeader()->setVisible(false);
        zaubertricksTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        zaubertricksTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_28->addWidget(zaubertricksTable_dsa5);

        label_30 = new QLabel(tabZauber_dsa5);
        label_30->setObjectName(QStringLiteral("label_30"));

        verticalLayout_28->addWidget(label_30);

        magiSonderfTable_dsa5 = new QTableWidget(tabZauber_dsa5);
        if (magiSonderfTable_dsa5->columnCount() < 1)
            magiSonderfTable_dsa5->setColumnCount(1);
        magiSonderfTable_dsa5->setObjectName(QStringLiteral("magiSonderfTable_dsa5"));
        magiSonderfTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        magiSonderfTable_dsa5->setColumnCount(1);
        magiSonderfTable_dsa5->horizontalHeader()->setVisible(false);
        magiSonderfTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        magiSonderfTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_28->addWidget(magiSonderfTable_dsa5);


        horizontalLayout_15->addLayout(verticalLayout_28);

        verticalLayout_29 = new QVBoxLayout();
        verticalLayout_29->setObjectName(QStringLiteral("verticalLayout_29"));
        leiteigMerkmalTable_dsa5 = new QTableWidget(tabZauber_dsa5);
        if (leiteigMerkmalTable_dsa5->columnCount() < 2)
            leiteigMerkmalTable_dsa5->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem243 = new QTableWidgetItem();
        leiteigMerkmalTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem243);
        QTableWidgetItem *__qtablewidgetitem244 = new QTableWidgetItem();
        leiteigMerkmalTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem244);
        if (leiteigMerkmalTable_dsa5->rowCount() < 1)
            leiteigMerkmalTable_dsa5->setRowCount(1);
        leiteigMerkmalTable_dsa5->setObjectName(QStringLiteral("leiteigMerkmalTable_dsa5"));
        leiteigMerkmalTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        leiteigMerkmalTable_dsa5->setRowCount(1);
        leiteigMerkmalTable_dsa5->horizontalHeader()->setDefaultSectionSize(300);
        leiteigMerkmalTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        leiteigMerkmalTable_dsa5->verticalHeader()->setVisible(false);
        leiteigMerkmalTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_29->addWidget(leiteigMerkmalTable_dsa5);

        mTraditionTable_dsa5 = new QTableWidget(tabZauber_dsa5);
        if (mTraditionTable_dsa5->columnCount() < 1)
            mTraditionTable_dsa5->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem245 = new QTableWidgetItem();
        mTraditionTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem245);
        if (mTraditionTable_dsa5->rowCount() < 1)
            mTraditionTable_dsa5->setRowCount(1);
        mTraditionTable_dsa5->setObjectName(QStringLiteral("mTraditionTable_dsa5"));
        mTraditionTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mTraditionTable_dsa5->setRowCount(1);
        mTraditionTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        mTraditionTable_dsa5->verticalHeader()->setVisible(false);
        mTraditionTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_29->addWidget(mTraditionTable_dsa5);


        horizontalLayout_15->addLayout(verticalLayout_29);


        verticalLayout_30->addLayout(horizontalLayout_15);

        tabWidget_2->addTab(tabZauber_dsa5, QString());
        tabLiturgien_dsa5 = new QWidget();
        tabLiturgien_dsa5->setObjectName(QStringLiteral("tabLiturgien_dsa5"));
        verticalLayout_25 = new QVBoxLayout(tabLiturgien_dsa5);
        verticalLayout_25->setObjectName(QStringLiteral("verticalLayout_25"));
        liturgienTable_dsa5 = new QTableWidget(tabLiturgien_dsa5);
        if (liturgienTable_dsa5->columnCount() < 11)
            liturgienTable_dsa5->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem246 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem246);
        QTableWidgetItem *__qtablewidgetitem247 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem247);
        QTableWidgetItem *__qtablewidgetitem248 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(2, __qtablewidgetitem248);
        QTableWidgetItem *__qtablewidgetitem249 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(3, __qtablewidgetitem249);
        QTableWidgetItem *__qtablewidgetitem250 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(4, __qtablewidgetitem250);
        QTableWidgetItem *__qtablewidgetitem251 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(5, __qtablewidgetitem251);
        QTableWidgetItem *__qtablewidgetitem252 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(6, __qtablewidgetitem252);
        QTableWidgetItem *__qtablewidgetitem253 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(7, __qtablewidgetitem253);
        QTableWidgetItem *__qtablewidgetitem254 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(8, __qtablewidgetitem254);
        QTableWidgetItem *__qtablewidgetitem255 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(9, __qtablewidgetitem255);
        QTableWidgetItem *__qtablewidgetitem256 = new QTableWidgetItem();
        liturgienTable_dsa5->setHorizontalHeaderItem(10, __qtablewidgetitem256);
        liturgienTable_dsa5->setObjectName(QStringLiteral("liturgienTable_dsa5"));
        liturgienTable_dsa5->setMinimumSize(QSize(0, 300));
        liturgienTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        liturgienTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        liturgienTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_25->addWidget(liturgienTable_dsa5);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        verticalLayout_27 = new QVBoxLayout();
        verticalLayout_27->setObjectName(QStringLiteral("verticalLayout_27"));
        label_26 = new QLabel(tabLiturgien_dsa5);
        label_26->setObjectName(QStringLiteral("label_26"));

        verticalLayout_27->addWidget(label_26);

        segnungenTable_dsa5 = new QTableWidget(tabLiturgien_dsa5);
        if (segnungenTable_dsa5->columnCount() < 1)
            segnungenTable_dsa5->setColumnCount(1);
        segnungenTable_dsa5->setObjectName(QStringLiteral("segnungenTable_dsa5"));
        segnungenTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        segnungenTable_dsa5->setColumnCount(1);
        segnungenTable_dsa5->horizontalHeader()->setVisible(false);
        segnungenTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        segnungenTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_27->addWidget(segnungenTable_dsa5);

        label_28 = new QLabel(tabLiturgien_dsa5);
        label_28->setObjectName(QStringLiteral("label_28"));

        verticalLayout_27->addWidget(label_28);

        kleriSonderfTable_dsa5 = new QTableWidget(tabLiturgien_dsa5);
        if (kleriSonderfTable_dsa5->columnCount() < 1)
            kleriSonderfTable_dsa5->setColumnCount(1);
        kleriSonderfTable_dsa5->setObjectName(QStringLiteral("kleriSonderfTable_dsa5"));
        kleriSonderfTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        kleriSonderfTable_dsa5->setColumnCount(1);
        kleriSonderfTable_dsa5->horizontalHeader()->setVisible(false);
        kleriSonderfTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        kleriSonderfTable_dsa5->verticalHeader()->setVisible(false);

        verticalLayout_27->addWidget(kleriSonderfTable_dsa5);


        horizontalLayout_14->addLayout(verticalLayout_27);

        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setObjectName(QStringLiteral("verticalLayout_26"));
        leiteigAspektTable_dsa5 = new QTableWidget(tabLiturgien_dsa5);
        if (leiteigAspektTable_dsa5->columnCount() < 2)
            leiteigAspektTable_dsa5->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem257 = new QTableWidgetItem();
        leiteigAspektTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem257);
        QTableWidgetItem *__qtablewidgetitem258 = new QTableWidgetItem();
        leiteigAspektTable_dsa5->setHorizontalHeaderItem(1, __qtablewidgetitem258);
        if (leiteigAspektTable_dsa5->rowCount() < 1)
            leiteigAspektTable_dsa5->setRowCount(1);
        leiteigAspektTable_dsa5->setObjectName(QStringLiteral("leiteigAspektTable_dsa5"));
        leiteigAspektTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        leiteigAspektTable_dsa5->setRowCount(1);
        leiteigAspektTable_dsa5->horizontalHeader()->setDefaultSectionSize(300);
        leiteigAspektTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        leiteigAspektTable_dsa5->verticalHeader()->setVisible(false);
        leiteigAspektTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_26->addWidget(leiteigAspektTable_dsa5);

        kTraditionTable_dsa5 = new QTableWidget(tabLiturgien_dsa5);
        if (kTraditionTable_dsa5->columnCount() < 1)
            kTraditionTable_dsa5->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem259 = new QTableWidgetItem();
        kTraditionTable_dsa5->setHorizontalHeaderItem(0, __qtablewidgetitem259);
        if (kTraditionTable_dsa5->rowCount() < 1)
            kTraditionTable_dsa5->setRowCount(1);
        kTraditionTable_dsa5->setObjectName(QStringLiteral("kTraditionTable_dsa5"));
        kTraditionTable_dsa5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        kTraditionTable_dsa5->setRowCount(1);
        kTraditionTable_dsa5->horizontalHeader()->setDefaultSectionSize(300);
        kTraditionTable_dsa5->horizontalHeader()->setStretchLastSection(true);
        kTraditionTable_dsa5->verticalHeader()->setVisible(false);
        kTraditionTable_dsa5->verticalHeader()->setStretchLastSection(true);

        verticalLayout_26->addWidget(kTraditionTable_dsa5);


        horizontalLayout_14->addLayout(verticalLayout_26);


        verticalLayout_25->addLayout(horizontalLayout_14);

        tabWidget_2->addTab(tabLiturgien_dsa5, QString());

        verticalLayout_8->addWidget(tabWidget_2);

        CharacterPage->addWidget(pageDSA5);

        retranslateUi(CharacterPage);

        CharacterPage->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CharacterPage);
    } // setupUi

    void retranslateUi(QStackedWidget *CharacterPage)
    {
        CharacterPage->setWindowTitle(QApplication::translate("CharacterPage", "StackedWidget", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = generalInfoTable_generic->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CharacterPage", "Name:", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = generalInfoTable_generic->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CharacterPage", "Surname:", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = generalInfoTable_generic->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CharacterPage", "Species:", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = generalInfoTable_generic->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CharacterPage", "Profession:", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = generalInfoTable_generic->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CharacterPage", "Culture:", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = generalInfoTable_generic->verticalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CharacterPage", "Height:", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = generalInfoTable_generic->verticalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CharacterPage", "Weight:", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabGeneral_generic), QApplication::translate("CharacterPage", "General Info", Q_NULLPTR));
        label->setText(QApplication::translate("CharacterPage", "Skills 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = skills1Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("CharacterPage", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = skills1Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QApplication::translate("CharacterPage", "Value", Q_NULLPTR));
        label_2->setText(QApplication::translate("CharacterPage", "Skills 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = skills2Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("CharacterPage", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = skills2Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("CharacterPage", "Value", Q_NULLPTR));
        label_3->setText(QApplication::translate("CharacterPage", "Skills 3", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = skills3Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QApplication::translate("CharacterPage", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = skills3Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QApplication::translate("CharacterPage", "Value", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSkills_generic), QApplication::translate("CharacterPage", "Skills", Q_NULLPTR));
        label_4->setText(QApplication::translate("CharacterPage", "Weapons", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = weaponsTable_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem13->setText(QApplication::translate("CharacterPage", "Name", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem14 = weaponsTable_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem14->setText(QApplication::translate("CharacterPage", "Type", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem15 = weaponsTable_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem15->setText(QApplication::translate("CharacterPage", "Damage", Q_NULLPTR));
        label_5->setText(QApplication::translate("CharacterPage", "Armor", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem16 = armorTable_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem16->setText(QApplication::translate("CharacterPage", "Type", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem17 = armorTable_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem17->setText(QApplication::translate("CharacterPage", "Protection", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem18 = armorTable_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem18->setText(QApplication::translate("CharacterPage", "Modifiers", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabFight_generic), QApplication::translate("CharacterPage", "Fight", Q_NULLPTR));
        label_6->setText(QApplication::translate("CharacterPage", "Inventory 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem19 = inv1Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem19->setText(QApplication::translate("CharacterPage", "Item", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem20 = inv1Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem20->setText(QApplication::translate("CharacterPage", "Where", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem21 = inv1Table_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem21->setText(QApplication::translate("CharacterPage", "Weight", Q_NULLPTR));
        label_7->setText(QApplication::translate("CharacterPage", "Inventory 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem22 = inv2Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem22->setText(QApplication::translate("CharacterPage", "Item", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem23 = inv2Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem23->setText(QApplication::translate("CharacterPage", "Where", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem24 = inv2Table_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem24->setText(QApplication::translate("CharacterPage", "Weight", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabInventory_generic), QApplication::translate("CharacterPage", "Inventory", Q_NULLPTR));
        dsa5Label->setText(QApplication::translate("CharacterPage", "DSA5", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem25 = eigenschaftenTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem25->setText(QApplication::translate("CharacterPage", "MU", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem26 = eigenschaftenTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem26->setText(QApplication::translate("CharacterPage", "KL", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem27 = eigenschaftenTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem27->setText(QApplication::translate("CharacterPage", "IN", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem28 = eigenschaftenTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem28->setText(QApplication::translate("CharacterPage", "CH", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem29 = eigenschaftenTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem29->setText(QApplication::translate("CharacterPage", "FF", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem30 = eigenschaftenTable_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem30->setText(QApplication::translate("CharacterPage", "GE", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem31 = eigenschaftenTable_dsa5->horizontalHeaderItem(6);
        ___qtablewidgetitem31->setText(QApplication::translate("CharacterPage", "KO", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem32 = eigenschaftenTable_dsa5->horizontalHeaderItem(7);
        ___qtablewidgetitem32->setText(QApplication::translate("CharacterPage", "KK", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem33 = persDaten1Table_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem33->setText(QApplication::translate("CharacterPage", "Name", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem34 = persDaten1Table_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem34->setText(QApplication::translate("CharacterPage", "Familie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem35 = persDaten1Table_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem35->setText(QApplication::translate("CharacterPage", "Geburtsdatum", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem36 = persDaten1Table_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem36->setText(QApplication::translate("CharacterPage", "Spezies", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem37 = persDaten1Table_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem37->setText(QApplication::translate("CharacterPage", "Haarfarbe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem38 = persDaten1Table_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem38->setText(QApplication::translate("CharacterPage", "Kultur", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem39 = persDaten1Table_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem39->setText(QApplication::translate("CharacterPage", "Titel", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem40 = persDaten1Table_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem40->setText(QApplication::translate("CharacterPage", "Charakteristika", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem41 = persDaten2Table_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem41->setText(QApplication::translate("CharacterPage", "Geburtsort", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem42 = persDaten2Table_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem42->setText(QApplication::translate("CharacterPage", "Alter", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem43 = persDaten2Table_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem43->setText(QApplication::translate("CharacterPage", "Geschlecht", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem44 = persDaten2Table_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem44->setText(QApplication::translate("CharacterPage", "Gr\303\266\303\237e", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem45 = persDaten2Table_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem45->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem46 = persDaten2Table_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem46->setText(QApplication::translate("CharacterPage", "Augenfarbe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem47 = persDaten2Table_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem47->setText(QApplication::translate("CharacterPage", "Profession", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem48 = persDaten2Table_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem48->setText(QApplication::translate("CharacterPage", "Sozialstatus", Q_NULLPTR));
        label_9->setText(QApplication::translate("CharacterPage", "Vorteile", Q_NULLPTR));
        label_8->setText(QApplication::translate("CharacterPage", "Nachteile", Q_NULLPTR));
        label_10->setText(QApplication::translate("CharacterPage", "Allgemeine Sonderfertigkeiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem49 = apTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem49->setText(QApplication::translate("CharacterPage", "Erfahrungsgrad", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem50 = apTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem50->setText(QApplication::translate("CharacterPage", "AP Gesamt", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem51 = apTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem51->setText(QApplication::translate("CharacterPage", "AP Gesammelt", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem52 = apTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem52->setText(QApplication::translate("CharacterPage", "AP Ausgegeben", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem53 = allgemeinTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem53->setText(QApplication::translate("CharacterPage", "Wert", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem54 = allgemeinTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem54->setText(QApplication::translate("CharacterPage", "Bonus/Malus", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem55 = allgemeinTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem55->setText(QApplication::translate("CharacterPage", "Zukauf", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem56 = allgemeinTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem56->setText(QApplication::translate("CharacterPage", "Max", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem57 = allgemeinTable_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem57->setText(QApplication::translate("CharacterPage", "Lebensenergie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem58 = allgemeinTable_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem58->setText(QApplication::translate("CharacterPage", "Astralenergie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem59 = allgemeinTable_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem59->setText(QApplication::translate("CharacterPage", "Karmaenergie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem60 = allgemeinTable_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem60->setText(QApplication::translate("CharacterPage", "Seelenkraft", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem61 = allgemeinTable_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem61->setText(QApplication::translate("CharacterPage", "Z\303\244higkeit", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem62 = allgemeinTable_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem62->setText(QApplication::translate("CharacterPage", "Ausweichen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem63 = allgemeinTable_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem63->setText(QApplication::translate("CharacterPage", "Initiative", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem64 = allgemeinTable_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem64->setText(QApplication::translate("CharacterPage", "Geschwindigkeit", Q_NULLPTR));
        label_11->setText(QApplication::translate("CharacterPage", "Schicksalspunkte", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem65 = schicksalspTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem65->setText(QApplication::translate("CharacterPage", "Wert", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem66 = schicksalspTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem66->setText(QApplication::translate("CharacterPage", "Bonus", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem67 = schicksalspTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem67->setText(QApplication::translate("CharacterPage", "Max", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem68 = schicksalspTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem68->setText(QApplication::translate("CharacterPage", "Aktuell", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabPersDaten_dsa5), QApplication::translate("CharacterPage", "Pers\303\266nliche Daten", Q_NULLPTR));
        label_12->setText(QApplication::translate("CharacterPage", "Fertigkeiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem69 = fertigkeiten1Table_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem69->setText(QApplication::translate("CharacterPage", "Probe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem70 = fertigkeiten1Table_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem70->setText(QApplication::translate("CharacterPage", "BE", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem71 = fertigkeiten1Table_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem71->setText(QApplication::translate("CharacterPage", "Stg.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem72 = fertigkeiten1Table_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem72->setText(QApplication::translate("CharacterPage", "Fw", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem73 = fertigkeiten1Table_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem73->setText(QApplication::translate("CharacterPage", "R", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem74 = fertigkeiten1Table_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem74->setText(QApplication::translate("CharacterPage", "Anmerkung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem75 = fertigkeiten1Table_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem75->setText(QApplication::translate("CharacterPage", "K\303\266rpertalente", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem76 = fertigkeiten1Table_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem76->setText(QApplication::translate("CharacterPage", "Fliegen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem77 = fertigkeiten1Table_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem77->setText(QApplication::translate("CharacterPage", "Gaukeleien", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem78 = fertigkeiten1Table_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem78->setText(QApplication::translate("CharacterPage", "Klettern", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem79 = fertigkeiten1Table_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem79->setText(QApplication::translate("CharacterPage", "K\303\266rperbeherrschung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem80 = fertigkeiten1Table_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem80->setText(QApplication::translate("CharacterPage", "Kraftakt", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem81 = fertigkeiten1Table_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem81->setText(QApplication::translate("CharacterPage", "Reiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem82 = fertigkeiten1Table_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem82->setText(QApplication::translate("CharacterPage", "Schwimmen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem83 = fertigkeiten1Table_dsa5->verticalHeaderItem(8);
        ___qtablewidgetitem83->setText(QApplication::translate("CharacterPage", "Selbstbeherrschung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem84 = fertigkeiten1Table_dsa5->verticalHeaderItem(9);
        ___qtablewidgetitem84->setText(QApplication::translate("CharacterPage", "Singen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem85 = fertigkeiten1Table_dsa5->verticalHeaderItem(10);
        ___qtablewidgetitem85->setText(QApplication::translate("CharacterPage", "Sinnessch\303\244rfe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem86 = fertigkeiten1Table_dsa5->verticalHeaderItem(11);
        ___qtablewidgetitem86->setText(QApplication::translate("CharacterPage", "Tanzen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem87 = fertigkeiten1Table_dsa5->verticalHeaderItem(12);
        ___qtablewidgetitem87->setText(QApplication::translate("CharacterPage", "Taschendiebstahl", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem88 = fertigkeiten1Table_dsa5->verticalHeaderItem(13);
        ___qtablewidgetitem88->setText(QApplication::translate("CharacterPage", "Verbergen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem89 = fertigkeiten1Table_dsa5->verticalHeaderItem(14);
        ___qtablewidgetitem89->setText(QApplication::translate("CharacterPage", "Zechen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem90 = fertigkeiten1Table_dsa5->verticalHeaderItem(16);
        ___qtablewidgetitem90->setText(QApplication::translate("CharacterPage", "Gesellschaftstalente", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem91 = fertigkeiten1Table_dsa5->verticalHeaderItem(17);
        ___qtablewidgetitem91->setText(QApplication::translate("CharacterPage", "Bekehren & \303\234berzeugen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem92 = fertigkeiten1Table_dsa5->verticalHeaderItem(18);
        ___qtablewidgetitem92->setText(QApplication::translate("CharacterPage", "Ber\303\266ren", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem93 = fertigkeiten1Table_dsa5->verticalHeaderItem(19);
        ___qtablewidgetitem93->setText(QApplication::translate("CharacterPage", "Einsch\303\274chtern", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem94 = fertigkeiten1Table_dsa5->verticalHeaderItem(20);
        ___qtablewidgetitem94->setText(QApplication::translate("CharacterPage", "Etikette", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem95 = fertigkeiten1Table_dsa5->verticalHeaderItem(21);
        ___qtablewidgetitem95->setText(QApplication::translate("CharacterPage", "Gassenwissen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem96 = fertigkeiten1Table_dsa5->verticalHeaderItem(22);
        ___qtablewidgetitem96->setText(QApplication::translate("CharacterPage", "Menschenkenntnis", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem97 = fertigkeiten1Table_dsa5->verticalHeaderItem(23);
        ___qtablewidgetitem97->setText(QApplication::translate("CharacterPage", "\303\234berreden", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem98 = fertigkeiten1Table_dsa5->verticalHeaderItem(24);
        ___qtablewidgetitem98->setText(QApplication::translate("CharacterPage", "Verkleiden", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem99 = fertigkeiten1Table_dsa5->verticalHeaderItem(25);
        ___qtablewidgetitem99->setText(QApplication::translate("CharacterPage", "Willenskraft", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem100 = fertigkeiten1Table_dsa5->verticalHeaderItem(27);
        ___qtablewidgetitem100->setText(QApplication::translate("CharacterPage", "Naturtalente", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem101 = fertigkeiten1Table_dsa5->verticalHeaderItem(28);
        ___qtablewidgetitem101->setText(QApplication::translate("CharacterPage", "F\303\244hrtensuchen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem102 = fertigkeiten1Table_dsa5->verticalHeaderItem(29);
        ___qtablewidgetitem102->setText(QApplication::translate("CharacterPage", "Fesseln", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem103 = fertigkeiten1Table_dsa5->verticalHeaderItem(30);
        ___qtablewidgetitem103->setText(QApplication::translate("CharacterPage", "Fischen & Angeln", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem104 = fertigkeiten1Table_dsa5->verticalHeaderItem(31);
        ___qtablewidgetitem104->setText(QApplication::translate("CharacterPage", "Orientierung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem105 = fertigkeiten1Table_dsa5->verticalHeaderItem(32);
        ___qtablewidgetitem105->setText(QApplication::translate("CharacterPage", "Pflanzenkunde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem106 = fertigkeiten1Table_dsa5->verticalHeaderItem(33);
        ___qtablewidgetitem106->setText(QApplication::translate("CharacterPage", "Tierkunde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem107 = fertigkeiten1Table_dsa5->verticalHeaderItem(34);
        ___qtablewidgetitem107->setText(QApplication::translate("CharacterPage", "Wildnisleben", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem108 = fertigkeiten2Table_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem108->setText(QApplication::translate("CharacterPage", "Probe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem109 = fertigkeiten2Table_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem109->setText(QApplication::translate("CharacterPage", "BE", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem110 = fertigkeiten2Table_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem110->setText(QApplication::translate("CharacterPage", "Stg.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem111 = fertigkeiten2Table_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem111->setText(QApplication::translate("CharacterPage", "Fw", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem112 = fertigkeiten2Table_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem112->setText(QApplication::translate("CharacterPage", "R", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem113 = fertigkeiten2Table_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem113->setText(QApplication::translate("CharacterPage", "Anmerkung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem114 = fertigkeiten2Table_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem114->setText(QApplication::translate("CharacterPage", "Wissenstalente", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem115 = fertigkeiten2Table_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem115->setText(QApplication::translate("CharacterPage", "Brett- & Gl\303\274cksspiel", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem116 = fertigkeiten2Table_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem116->setText(QApplication::translate("CharacterPage", "Geographie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem117 = fertigkeiten2Table_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem117->setText(QApplication::translate("CharacterPage", "Geschichtswissen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem118 = fertigkeiten2Table_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem118->setText(QApplication::translate("CharacterPage", "G\303\266tter & Kulte", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem119 = fertigkeiten2Table_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem119->setText(QApplication::translate("CharacterPage", "Kriegskunst", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem120 = fertigkeiten2Table_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem120->setText(QApplication::translate("CharacterPage", "Magiekunde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem121 = fertigkeiten2Table_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem121->setText(QApplication::translate("CharacterPage", "Mechanik", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem122 = fertigkeiten2Table_dsa5->verticalHeaderItem(8);
        ___qtablewidgetitem122->setText(QApplication::translate("CharacterPage", "Rechnen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem123 = fertigkeiten2Table_dsa5->verticalHeaderItem(9);
        ___qtablewidgetitem123->setText(QApplication::translate("CharacterPage", "Rechtskunde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem124 = fertigkeiten2Table_dsa5->verticalHeaderItem(10);
        ___qtablewidgetitem124->setText(QApplication::translate("CharacterPage", "Sagen & Legenden", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem125 = fertigkeiten2Table_dsa5->verticalHeaderItem(11);
        ___qtablewidgetitem125->setText(QApplication::translate("CharacterPage", "Sph\303\244renkunde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem126 = fertigkeiten2Table_dsa5->verticalHeaderItem(12);
        ___qtablewidgetitem126->setText(QApplication::translate("CharacterPage", "Sternkunde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem127 = fertigkeiten2Table_dsa5->verticalHeaderItem(14);
        ___qtablewidgetitem127->setText(QApplication::translate("CharacterPage", "Handwerkstalente", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem128 = fertigkeiten2Table_dsa5->verticalHeaderItem(15);
        ___qtablewidgetitem128->setText(QApplication::translate("CharacterPage", "Alchimie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem129 = fertigkeiten2Table_dsa5->verticalHeaderItem(16);
        ___qtablewidgetitem129->setText(QApplication::translate("CharacterPage", "Boote & Schiffe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem130 = fertigkeiten2Table_dsa5->verticalHeaderItem(17);
        ___qtablewidgetitem130->setText(QApplication::translate("CharacterPage", "Fahrzeuge", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem131 = fertigkeiten2Table_dsa5->verticalHeaderItem(18);
        ___qtablewidgetitem131->setText(QApplication::translate("CharacterPage", "Handel", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem132 = fertigkeiten2Table_dsa5->verticalHeaderItem(19);
        ___qtablewidgetitem132->setText(QApplication::translate("CharacterPage", "Heilkunde Gift", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem133 = fertigkeiten2Table_dsa5->verticalHeaderItem(20);
        ___qtablewidgetitem133->setText(QApplication::translate("CharacterPage", "Heilkunde Krankheiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem134 = fertigkeiten2Table_dsa5->verticalHeaderItem(21);
        ___qtablewidgetitem134->setText(QApplication::translate("CharacterPage", "Heilkunde Seele", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem135 = fertigkeiten2Table_dsa5->verticalHeaderItem(22);
        ___qtablewidgetitem135->setText(QApplication::translate("CharacterPage", "Heilkunde Wunden", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem136 = fertigkeiten2Table_dsa5->verticalHeaderItem(23);
        ___qtablewidgetitem136->setText(QApplication::translate("CharacterPage", "Holzbearbeitung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem137 = fertigkeiten2Table_dsa5->verticalHeaderItem(24);
        ___qtablewidgetitem137->setText(QApplication::translate("CharacterPage", "Lebensmittelbearbeitung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem138 = fertigkeiten2Table_dsa5->verticalHeaderItem(25);
        ___qtablewidgetitem138->setText(QApplication::translate("CharacterPage", "Lederbearbeitung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem139 = fertigkeiten2Table_dsa5->verticalHeaderItem(26);
        ___qtablewidgetitem139->setText(QApplication::translate("CharacterPage", "Malen & Zeichnen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem140 = fertigkeiten2Table_dsa5->verticalHeaderItem(27);
        ___qtablewidgetitem140->setText(QApplication::translate("CharacterPage", "Metallbearbeitung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem141 = fertigkeiten2Table_dsa5->verticalHeaderItem(28);
        ___qtablewidgetitem141->setText(QApplication::translate("CharacterPage", "Musizieren", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem142 = fertigkeiten2Table_dsa5->verticalHeaderItem(29);
        ___qtablewidgetitem142->setText(QApplication::translate("CharacterPage", "Schl\303\266sserknacken", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem143 = fertigkeiten2Table_dsa5->verticalHeaderItem(30);
        ___qtablewidgetitem143->setText(QApplication::translate("CharacterPage", "Steinbearbeitung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem144 = fertigkeiten2Table_dsa5->verticalHeaderItem(31);
        ___qtablewidgetitem144->setText(QApplication::translate("CharacterPage", "Stoffbearbeitung", Q_NULLPTR));
        label_13->setText(QApplication::translate("CharacterPage", "Sprachen", Q_NULLPTR));
        label_14->setText(QApplication::translate("CharacterPage", "Schriften", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabSpielwerte_dsa5), QApplication::translate("CharacterPage", "Spielwerte", Q_NULLPTR));
        label_15->setText(QApplication::translate("CharacterPage", "Kampftechniken", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem145 = ktwTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem145->setText(QApplication::translate("CharacterPage", "Leiteig.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem146 = ktwTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem146->setText(QApplication::translate("CharacterPage", "Sf.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem147 = ktwTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem147->setText(QApplication::translate("CharacterPage", "Ktw.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem148 = ktwTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem148->setText(QApplication::translate("CharacterPage", "AT/FK", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem149 = ktwTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem149->setText(QApplication::translate("CharacterPage", "PA", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem150 = ktwTable_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem150->setText(QApplication::translate("CharacterPage", "Armbr\303\274ste", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem151 = ktwTable_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem151->setText(QApplication::translate("CharacterPage", "B\303\266gen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem152 = ktwTable_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem152->setText(QApplication::translate("CharacterPage", "Dolche", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem153 = ktwTable_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem153->setText(QApplication::translate("CharacterPage", "Fechtwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem154 = ktwTable_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem154->setText(QApplication::translate("CharacterPage", "Hiebwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem155 = ktwTable_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem155->setText(QApplication::translate("CharacterPage", "Kettenwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem156 = ktwTable_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem156->setText(QApplication::translate("CharacterPage", "Raufen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem157 = ktwTable_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem157->setText(QApplication::translate("CharacterPage", "Schilde", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem158 = ktwTable_dsa5->verticalHeaderItem(8);
        ___qtablewidgetitem158->setText(QApplication::translate("CharacterPage", "Schwerter", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem159 = ktwTable_dsa5->verticalHeaderItem(9);
        ___qtablewidgetitem159->setText(QApplication::translate("CharacterPage", "Stangenwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem160 = ktwTable_dsa5->verticalHeaderItem(10);
        ___qtablewidgetitem160->setText(QApplication::translate("CharacterPage", "Wurfwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem161 = ktwTable_dsa5->verticalHeaderItem(11);
        ___qtablewidgetitem161->setText(QApplication::translate("CharacterPage", "Zweihandhiebwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem162 = ktwTable_dsa5->verticalHeaderItem(12);
        ___qtablewidgetitem162->setText(QApplication::translate("CharacterPage", "Zweihandschwerter", Q_NULLPTR));
        label_16->setText(QApplication::translate("CharacterPage", "Lebensenergie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem163 = lepTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem163->setText(QApplication::translate("CharacterPage", "Max", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem164 = lepTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem164->setText(QApplication::translate("CharacterPage", "Aktuell", Q_NULLPTR));

        const bool __sortingEnabled = lepTable_dsa5->isSortingEnabled();
        lepTable_dsa5->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem165 = lepTable_dsa5->item(0, 0);
        ___qtablewidgetitem165->setText(QApplication::translate("CharacterPage", "1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem166 = lepTable_dsa5->item(0, 1);
        ___qtablewidgetitem166->setText(QApplication::translate("CharacterPage", "1", Q_NULLPTR));
        lepTable_dsa5->setSortingEnabled(__sortingEnabled);

        label_17->setText(QApplication::translate("CharacterPage", "Kampfsonderfertigkeiten", Q_NULLPTR));
        label_19->setText(QApplication::translate("CharacterPage", "Nahkampfwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem167 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem167->setText(QApplication::translate("CharacterPage", "Waffe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem168 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem168->setText(QApplication::translate("CharacterPage", "Kampftechnik", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem169 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem169->setText(QApplication::translate("CharacterPage", "Schadensb.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem170 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem170->setText(QApplication::translate("CharacterPage", "TP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem171 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem171->setText(QApplication::translate("CharacterPage", "AT/PA Mod.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem172 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem172->setText(QApplication::translate("CharacterPage", "Reichweite", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem173 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(6);
        ___qtablewidgetitem173->setText(QApplication::translate("CharacterPage", "AT", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem174 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(7);
        ___qtablewidgetitem174->setText(QApplication::translate("CharacterPage", "PA", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem175 = nahkampfwaffenTable_dsa5->horizontalHeaderItem(8);
        ___qtablewidgetitem175->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        label_20->setText(QApplication::translate("CharacterPage", "Fernkampfwaffen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem176 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem176->setText(QApplication::translate("CharacterPage", "Waffe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem177 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem177->setText(QApplication::translate("CharacterPage", "Kampftechnik", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem178 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem178->setText(QApplication::translate("CharacterPage", "Ladezeiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem179 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem179->setText(QApplication::translate("CharacterPage", "TP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem180 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem180->setText(QApplication::translate("CharacterPage", "Reichweite", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem181 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem181->setText(QApplication::translate("CharacterPage", "Fernkampf", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem182 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(6);
        ___qtablewidgetitem182->setText(QApplication::translate("CharacterPage", "Munition", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem183 = fernkampfwaffenTable_dsa5->horizontalHeaderItem(7);
        ___qtablewidgetitem183->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        label_18->setText(QApplication::translate("CharacterPage", "R\303\274stungen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem184 = ruestungenTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem184->setText(QApplication::translate("CharacterPage", "R\303\274stung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem185 = ruestungenTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem185->setText(QApplication::translate("CharacterPage", "RS", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem186 = ruestungenTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem186->setText(QApplication::translate("CharacterPage", "BE", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem187 = ruestungenTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem187->setText(QApplication::translate("CharacterPage", "Zus. Abz\303\274ge", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem188 = ruestungenTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem188->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem189 = ruestungenTable_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem189->setText(QApplication::translate("CharacterPage", "Reise, Schlacht, ...", Q_NULLPTR));
        label_21->setText(QApplication::translate("CharacterPage", "Schild / Parierwaffe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem190 = schildTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem190->setText(QApplication::translate("CharacterPage", "Schild / Parierwaffe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem191 = schildTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem191->setText(QApplication::translate("CharacterPage", "Strukturp.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem192 = schildTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem192->setText(QApplication::translate("CharacterPage", "AT/PA Mod.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem193 = schildTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem193->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabKampf_dsa5), QApplication::translate("CharacterPage", "Kampf", Q_NULLPTR));
        label_22->setText(QApplication::translate("CharacterPage", "Ausr\303\274stung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem194 = ausruestung1Table_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem194->setText(QApplication::translate("CharacterPage", "Gegenstand", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem195 = ausruestung1Table_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem195->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem196 = ausruestung1Table_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem196->setText(QApplication::translate("CharacterPage", "Wo getragen", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem197 = ausruestung2Table_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem197->setText(QApplication::translate("CharacterPage", "Gegenstand", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem198 = ausruestung2Table_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem198->setText(QApplication::translate("CharacterPage", "Gewicht", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem199 = ausruestung2Table_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem199->setText(QApplication::translate("CharacterPage", "Wo getragen", Q_NULLPTR));
        label_23->setText(QApplication::translate("CharacterPage", "Geldbeutel", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem200 = geldTable_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem200->setText(QApplication::translate("CharacterPage", "Dukaten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem201 = geldTable_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem201->setText(QApplication::translate("CharacterPage", "Silbertaler", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem202 = geldTable_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem202->setText(QApplication::translate("CharacterPage", "Heller", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem203 = geldTable_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem203->setText(QApplication::translate("CharacterPage", "Kreuzer", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem204 = geldTable_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem204->setText(QApplication::translate("CharacterPage", "Edelsteine", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem205 = geldTable_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem205->setText(QApplication::translate("CharacterPage", "Schmuck", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem206 = geldTable_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem206->setText(QApplication::translate("CharacterPage", "Sonstiges", Q_NULLPTR));
        label_25->setText(QApplication::translate("CharacterPage", "Tier-Sonderfertigkeiten", Q_NULLPTR));
        label_24->setText(QApplication::translate("CharacterPage", "Tier", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem207 = tierAllgemeinTable_dsa5->verticalHeaderItem(0);
        ___qtablewidgetitem207->setText(QApplication::translate("CharacterPage", "Name", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem208 = tierAllgemeinTable_dsa5->verticalHeaderItem(1);
        ___qtablewidgetitem208->setText(QApplication::translate("CharacterPage", "Typus", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem209 = tierAllgemeinTable_dsa5->verticalHeaderItem(2);
        ___qtablewidgetitem209->setText(QApplication::translate("CharacterPage", "Gr\303\266\303\237enkategorie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem210 = tierAllgemeinTable_dsa5->verticalHeaderItem(3);
        ___qtablewidgetitem210->setText(QApplication::translate("CharacterPage", "MU", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem211 = tierAllgemeinTable_dsa5->verticalHeaderItem(4);
        ___qtablewidgetitem211->setText(QApplication::translate("CharacterPage", "KL", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem212 = tierAllgemeinTable_dsa5->verticalHeaderItem(5);
        ___qtablewidgetitem212->setText(QApplication::translate("CharacterPage", "IN", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem213 = tierAllgemeinTable_dsa5->verticalHeaderItem(6);
        ___qtablewidgetitem213->setText(QApplication::translate("CharacterPage", "CH", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem214 = tierAllgemeinTable_dsa5->verticalHeaderItem(7);
        ___qtablewidgetitem214->setText(QApplication::translate("CharacterPage", "FF", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem215 = tierAllgemeinTable_dsa5->verticalHeaderItem(8);
        ___qtablewidgetitem215->setText(QApplication::translate("CharacterPage", "GE", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem216 = tierAllgemeinTable_dsa5->verticalHeaderItem(9);
        ___qtablewidgetitem216->setText(QApplication::translate("CharacterPage", "KO", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem217 = tierAllgemeinTable_dsa5->verticalHeaderItem(10);
        ___qtablewidgetitem217->setText(QApplication::translate("CharacterPage", "KK", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem218 = tierAllgemeinTable_dsa5->verticalHeaderItem(11);
        ___qtablewidgetitem218->setText(QApplication::translate("CharacterPage", "LeP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem219 = tierAllgemeinTable_dsa5->verticalHeaderItem(12);
        ___qtablewidgetitem219->setText(QApplication::translate("CharacterPage", "AsP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem220 = tierAllgemeinTable_dsa5->verticalHeaderItem(13);
        ___qtablewidgetitem220->setText(QApplication::translate("CharacterPage", "SK", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem221 = tierAllgemeinTable_dsa5->verticalHeaderItem(14);
        ___qtablewidgetitem221->setText(QApplication::translate("CharacterPage", "ZK", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem222 = tierAllgemeinTable_dsa5->verticalHeaderItem(15);
        ___qtablewidgetitem222->setText(QApplication::translate("CharacterPage", "RS", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem223 = tierAllgemeinTable_dsa5->verticalHeaderItem(16);
        ___qtablewidgetitem223->setText(QApplication::translate("CharacterPage", "INI", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem224 = tierAllgemeinTable_dsa5->verticalHeaderItem(17);
        ___qtablewidgetitem224->setText(QApplication::translate("CharacterPage", "GS", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem225 = tierAngrifTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem225->setText(QApplication::translate("CharacterPage", "Angriff", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem226 = tierAngrifTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem226->setText(QApplication::translate("CharacterPage", "AT", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem227 = tierAngrifTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem227->setText(QApplication::translate("CharacterPage", "PA", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem228 = tierAngrifTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem228->setText(QApplication::translate("CharacterPage", "TP", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem229 = tierAngrifTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem229->setText(QApplication::translate("CharacterPage", "RW", Q_NULLPTR));
        label_27->setText(QApplication::translate("CharacterPage", "Aktionen", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabBesitz_dsa5), QApplication::translate("CharacterPage", "Besitz", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem230 = zauberTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem230->setText(QApplication::translate("CharacterPage", "Zaube / Ritual", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem231 = zauberTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem231->setText(QApplication::translate("CharacterPage", "Probe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem232 = zauberTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem232->setText(QApplication::translate("CharacterPage", "Fw", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem233 = zauberTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem233->setText(QApplication::translate("CharacterPage", "Zauberdauer", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem234 = zauberTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem234->setText(QApplication::translate("CharacterPage", "Reichweite", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem235 = zauberTable_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem235->setText(QApplication::translate("CharacterPage", "Wirkungsdauer", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem236 = zauberTable_dsa5->horizontalHeaderItem(6);
        ___qtablewidgetitem236->setText(QApplication::translate("CharacterPage", "Merkmal", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem237 = zauberTable_dsa5->horizontalHeaderItem(7);
        ___qtablewidgetitem237->setText(QApplication::translate("CharacterPage", "Sf.", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem238 = zauberTable_dsa5->horizontalHeaderItem(8);
        ___qtablewidgetitem238->setText(QApplication::translate("CharacterPage", "Wirkung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem239 = zauberTable_dsa5->horizontalHeaderItem(9);
        ___qtablewidgetitem239->setText(QApplication::translate("CharacterPage", "S.", Q_NULLPTR));
        label_29->setText(QApplication::translate("CharacterPage", "Zaubertricks", Q_NULLPTR));
        label_30->setText(QApplication::translate("CharacterPage", "Magische Sonderfertigkeiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem240 = leiteigMerkmalTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem240->setText(QApplication::translate("CharacterPage", "Leiteigenschaft", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem241 = leiteigMerkmalTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem241->setText(QApplication::translate("CharacterPage", "Merkmal", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem242 = mTraditionTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem242->setText(QApplication::translate("CharacterPage", "Tradition", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabZauber_dsa5), QApplication::translate("CharacterPage", "Zauber und Rituale", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem243 = liturgienTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem243->setText(QApplication::translate("CharacterPage", "Liturgie / Zeremonie", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem244 = liturgienTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem244->setText(QApplication::translate("CharacterPage", "Probe", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem245 = liturgienTable_dsa5->horizontalHeaderItem(2);
        ___qtablewidgetitem245->setText(QApplication::translate("CharacterPage", "Fw", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem246 = liturgienTable_dsa5->horizontalHeaderItem(3);
        ___qtablewidgetitem246->setText(QApplication::translate("CharacterPage", "Kosten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem247 = liturgienTable_dsa5->horizontalHeaderItem(4);
        ___qtablewidgetitem247->setText(QApplication::translate("CharacterPage", "Liturgiedauer", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem248 = liturgienTable_dsa5->horizontalHeaderItem(5);
        ___qtablewidgetitem248->setText(QApplication::translate("CharacterPage", "Reichweite", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem249 = liturgienTable_dsa5->horizontalHeaderItem(6);
        ___qtablewidgetitem249->setText(QApplication::translate("CharacterPage", "Wirkungsdauer", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem250 = liturgienTable_dsa5->horizontalHeaderItem(7);
        ___qtablewidgetitem250->setText(QApplication::translate("CharacterPage", "Aspekt", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem251 = liturgienTable_dsa5->horizontalHeaderItem(8);
        ___qtablewidgetitem251->setText(QApplication::translate("CharacterPage", "Sf", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem252 = liturgienTable_dsa5->horizontalHeaderItem(9);
        ___qtablewidgetitem252->setText(QApplication::translate("CharacterPage", "Wirkung", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem253 = liturgienTable_dsa5->horizontalHeaderItem(10);
        ___qtablewidgetitem253->setText(QApplication::translate("CharacterPage", "S", Q_NULLPTR));
        label_26->setText(QApplication::translate("CharacterPage", "Segnungen", Q_NULLPTR));
        label_28->setText(QApplication::translate("CharacterPage", "Klerikale Sonderfertigkeiten", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem254 = leiteigAspektTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem254->setText(QApplication::translate("CharacterPage", "Leiteigenschaft", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem255 = leiteigAspektTable_dsa5->horizontalHeaderItem(1);
        ___qtablewidgetitem255->setText(QApplication::translate("CharacterPage", "Aspekt", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem256 = kTraditionTable_dsa5->horizontalHeaderItem(0);
        ___qtablewidgetitem256->setText(QApplication::translate("CharacterPage", "Tradition", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tabLiturgien_dsa5), QApplication::translate("CharacterPage", "Liturgien und Zeremonien", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CharacterPage: public Ui_CharacterPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARACTERPAGE_H
