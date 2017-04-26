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
    QWidget *tab;
    QWidget *tab_2;

    void setupUi(QStackedWidget *CharacterPage)
    {
        if (CharacterPage->objectName().isEmpty())
            CharacterPage->setObjectName(QStringLiteral("CharacterPage"));
        CharacterPage->resize(706, 658);
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
        if (generalInfoTable_generic->rowCount() < 1)
            generalInfoTable_generic->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        generalInfoTable_generic->setVerticalHeaderItem(0, __qtablewidgetitem);
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
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        skills1Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        skills1Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem2);
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
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        skills2Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        skills2Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem4);
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
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        skills3Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        skills3Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem6);
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
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        weaponsTable_generic->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        weaponsTable_generic->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        weaponsTable_generic->setHorizontalHeaderItem(2, __qtablewidgetitem9);
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
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        armorTable_generic->setHorizontalHeaderItem(0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        armorTable_generic->setHorizontalHeaderItem(1, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        armorTable_generic->setHorizontalHeaderItem(2, __qtablewidgetitem12);
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
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        inv1Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        inv1Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        inv1Table_generic->setHorizontalHeaderItem(2, __qtablewidgetitem15);
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
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        inv2Table_generic->setHorizontalHeaderItem(0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        inv2Table_generic->setHorizontalHeaderItem(1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        inv2Table_generic->setHorizontalHeaderItem(2, __qtablewidgetitem18);
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
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget_2->addTab(tab_2, QString());

        verticalLayout_8->addWidget(tabWidget_2);

        CharacterPage->addWidget(pageDSA5);

        retranslateUi(CharacterPage);

        CharacterPage->setCurrentIndex(1);
        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(CharacterPage);
    } // setupUi

    void retranslateUi(QStackedWidget *CharacterPage)
    {
        CharacterPage->setWindowTitle(QApplication::translate("CharacterPage", "StackedWidget", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabGeneral_generic), QApplication::translate("CharacterPage", "General Info", Q_NULLPTR));
        label->setText(QApplication::translate("CharacterPage", "Skills 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = skills1Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CharacterPage", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = skills1Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CharacterPage", "Value", Q_NULLPTR));
        label_2->setText(QApplication::translate("CharacterPage", "Skills 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = skills2Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("CharacterPage", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = skills2Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("CharacterPage", "Value", Q_NULLPTR));
        label_3->setText(QApplication::translate("CharacterPage", "Skills 3", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = skills3Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("CharacterPage", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = skills3Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("CharacterPage", "Value", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSkills_generic), QApplication::translate("CharacterPage", "Skills", Q_NULLPTR));
        label_4->setText(QApplication::translate("CharacterPage", "Weapons", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = weaponsTable_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("CharacterPage", "Name", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = weaponsTable_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("CharacterPage", "Type", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = weaponsTable_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("CharacterPage", "Damage", Q_NULLPTR));
        label_5->setText(QApplication::translate("CharacterPage", "Armor", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = armorTable_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("CharacterPage", "Type", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = armorTable_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("CharacterPage", "Protection", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = armorTable_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem11->setText(QApplication::translate("CharacterPage", "Modifiers", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabFight_generic), QApplication::translate("CharacterPage", "Fight", Q_NULLPTR));
        label_6->setText(QApplication::translate("CharacterPage", "Inventory 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = inv1Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("CharacterPage", "Item", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = inv1Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("CharacterPage", "Where", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem14 = inv1Table_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("CharacterPage", "Weight", Q_NULLPTR));
        label_7->setText(QApplication::translate("CharacterPage", "Inventory 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem15 = inv2Table_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QApplication::translate("CharacterPage", "Item", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem16 = inv2Table_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QApplication::translate("CharacterPage", "Where", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem17 = inv2Table_generic->horizontalHeaderItem(2);
        ___qtablewidgetitem17->setText(QApplication::translate("CharacterPage", "Weight", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabInventory_generic), QApplication::translate("CharacterPage", "Inventory", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QApplication::translate("CharacterPage", "Pers\303\266nliche Daten", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("CharacterPage", "Spielwerte", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CharacterPage: public Ui_CharacterPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARACTERPAGE_H
