/********************************************************************************
** Form generated from reading UI file 'chareditor.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAREDITOR_H
#define UI_CHAREDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CharEditor
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QComboBox *systemComboBox;
    QLabel *label;
    QLineEdit *displayNameLineEdit;
    QLabel *label_2;
    QLineEdit *playerLineEdit;
    QFrame *line;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton_3;
    QFrame *line_2;
    QSpacerItem *verticalSpacer;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QStackedWidget *stackedWidget;
    QWidget *pageGeneric;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    QTableWidget *generalInfoTable;
    QSpacerItem *horizontalSpacer;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_5;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_6;
    QTableWidget *skillsTable1_generic;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *addRowSkillsTable1_generic;
    QPushButton *removeRowSkillsTable1_generic;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_7;
    QTableWidget *skillsTable2_generic;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *addRowSkillsTable2_generic;
    QPushButton *removeRowSkillsTable2_generic;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_8;
    QTableWidget *skillsTable3_generic;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *addRowSkillsTable3_generic;
    QPushButton *removeRowSkillsTable3_generic;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_6;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_9;
    QTableWidget *tableWidget_5;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *pushButton_9;
    QPushButton *pushButton_8;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_10;
    QTableWidget *tableWidget_4;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_10;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_11;
    QFrame *frame_8;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_11;
    QTableWidget *tableWidget_6;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_12;
    QFrame *frame_9;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_12;
    QTableWidget *tableWidget_7;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *pushButton_15;
    QPushButton *pushButton_14;
    QWidget *pageDSA5;

    void setupUi(QMainWindow *CharEditor)
    {
        if (CharEditor->objectName().isEmpty())
            CharEditor->setObjectName(QStringLiteral("CharEditor"));
        CharEditor->resize(1280, 720);
        centralwidget = new QWidget(CharEditor);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMaximumSize(QSize(200, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        systemComboBox = new QComboBox(frame);
        systemComboBox->setObjectName(QStringLiteral("systemComboBox"));

        verticalLayout->addWidget(systemComboBox);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        displayNameLineEdit = new QLineEdit(frame);
        displayNameLineEdit->setObjectName(QStringLiteral("displayNameLineEdit"));

        verticalLayout->addWidget(displayNameLineEdit);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        playerLineEdit = new QLineEdit(frame);
        playerLineEdit->setObjectName(QStringLiteral("playerLineEdit"));

        verticalLayout->addWidget(playerLineEdit);

        line = new QFrame(frame);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout->addWidget(label_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lineEdit_3 = new QLineEdit(frame);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_2->addWidget(lineEdit_3);

        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_2->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout_2);

        line_2 = new QFrame(frame);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        saveButton = new QPushButton(frame);
        saveButton->setObjectName(QStringLiteral("saveButton"));

        verticalLayout->addWidget(saveButton);

        cancelButton = new QPushButton(frame);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        verticalLayout->addWidget(cancelButton);


        horizontalLayout->addWidget(frame);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        pageGeneric = new QWidget();
        pageGeneric->setObjectName(QStringLiteral("pageGeneric"));
        horizontalLayout_3 = new QHBoxLayout(pageGeneric);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tabWidget = new QTabWidget(pageGeneric);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_2->addWidget(label_5);

        generalInfoTable = new QTableWidget(frame_2);
        if (generalInfoTable->columnCount() < 1)
            generalInfoTable->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        generalInfoTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (generalInfoTable->rowCount() < 7)
            generalInfoTable->setRowCount(7);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(5, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        generalInfoTable->setVerticalHeaderItem(6, __qtablewidgetitem7);
        generalInfoTable->setObjectName(QStringLiteral("generalInfoTable"));
        generalInfoTable->setAlternatingRowColors(false);
        generalInfoTable->setCornerButtonEnabled(false);
        generalInfoTable->horizontalHeader()->setVisible(false);
        generalInfoTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        generalInfoTable->horizontalHeader()->setStretchLastSection(true);
        generalInfoTable->verticalHeader()->setVisible(false);
        generalInfoTable->verticalHeader()->setCascadingSectionResizes(true);
        generalInfoTable->verticalHeader()->setDefaultSectionSize(46);
        generalInfoTable->verticalHeader()->setMinimumSectionSize(20);
        generalInfoTable->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        generalInfoTable->verticalHeader()->setStretchLastSection(false);

        verticalLayout_2->addWidget(generalInfoTable);


        horizontalLayout_4->addWidget(frame_2);

        horizontalSpacer = new QSpacerItem(502, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_5 = new QHBoxLayout(tab_2);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        frame_3 = new QFrame(tab_2);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_6 = new QLabel(frame_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_3->addWidget(label_6);

        skillsTable1_generic = new QTableWidget(frame_3);
        if (skillsTable1_generic->columnCount() < 2)
            skillsTable1_generic->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        skillsTable1_generic->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        skillsTable1_generic->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        skillsTable1_generic->setObjectName(QStringLiteral("skillsTable1_generic"));
        skillsTable1_generic->setAlternatingRowColors(true);
        skillsTable1_generic->horizontalHeader()->setCascadingSectionResizes(false);
        skillsTable1_generic->horizontalHeader()->setDefaultSectionSize(150);
        skillsTable1_generic->horizontalHeader()->setMinimumSectionSize(30);
        skillsTable1_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(skillsTable1_generic);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        addRowSkillsTable1_generic = new QPushButton(frame_3);
        addRowSkillsTable1_generic->setObjectName(QStringLiteral("addRowSkillsTable1_generic"));

        horizontalLayout_6->addWidget(addRowSkillsTable1_generic);

        removeRowSkillsTable1_generic = new QPushButton(frame_3);
        removeRowSkillsTable1_generic->setObjectName(QStringLiteral("removeRowSkillsTable1_generic"));

        horizontalLayout_6->addWidget(removeRowSkillsTable1_generic);


        verticalLayout_3->addLayout(horizontalLayout_6);


        horizontalLayout_5->addWidget(frame_3);

        frame_4 = new QFrame(tab_2);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_4->addWidget(label_7);

        skillsTable2_generic = new QTableWidget(frame_4);
        if (skillsTable2_generic->columnCount() < 2)
            skillsTable2_generic->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        skillsTable2_generic->setHorizontalHeaderItem(0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        skillsTable2_generic->setHorizontalHeaderItem(1, __qtablewidgetitem11);
        skillsTable2_generic->setObjectName(QStringLiteral("skillsTable2_generic"));
        skillsTable2_generic->setAlternatingRowColors(true);
        skillsTable2_generic->horizontalHeader()->setCascadingSectionResizes(false);
        skillsTable2_generic->horizontalHeader()->setDefaultSectionSize(150);
        skillsTable2_generic->horizontalHeader()->setMinimumSectionSize(30);
        skillsTable2_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_4->addWidget(skillsTable2_generic);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        addRowSkillsTable2_generic = new QPushButton(frame_4);
        addRowSkillsTable2_generic->setObjectName(QStringLiteral("addRowSkillsTable2_generic"));

        horizontalLayout_7->addWidget(addRowSkillsTable2_generic);

        removeRowSkillsTable2_generic = new QPushButton(frame_4);
        removeRowSkillsTable2_generic->setObjectName(QStringLiteral("removeRowSkillsTable2_generic"));

        horizontalLayout_7->addWidget(removeRowSkillsTable2_generic);


        verticalLayout_4->addLayout(horizontalLayout_7);


        horizontalLayout_5->addWidget(frame_4);

        frame_5 = new QFrame(tab_2);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_5);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_8 = new QLabel(frame_5);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_5->addWidget(label_8);

        skillsTable3_generic = new QTableWidget(frame_5);
        if (skillsTable3_generic->columnCount() < 2)
            skillsTable3_generic->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        skillsTable3_generic->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        skillsTable3_generic->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        skillsTable3_generic->setObjectName(QStringLiteral("skillsTable3_generic"));
        skillsTable3_generic->setAlternatingRowColors(true);
        skillsTable3_generic->horizontalHeader()->setCascadingSectionResizes(false);
        skillsTable3_generic->horizontalHeader()->setDefaultSectionSize(150);
        skillsTable3_generic->horizontalHeader()->setMinimumSectionSize(30);
        skillsTable3_generic->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_5->addWidget(skillsTable3_generic);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        addRowSkillsTable3_generic = new QPushButton(frame_5);
        addRowSkillsTable3_generic->setObjectName(QStringLiteral("addRowSkillsTable3_generic"));

        horizontalLayout_8->addWidget(addRowSkillsTable3_generic);

        removeRowSkillsTable3_generic = new QPushButton(frame_5);
        removeRowSkillsTable3_generic->setObjectName(QStringLiteral("removeRowSkillsTable3_generic"));

        horizontalLayout_8->addWidget(removeRowSkillsTable3_generic);


        verticalLayout_5->addLayout(horizontalLayout_8);


        horizontalLayout_5->addWidget(frame_5);

        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_6 = new QVBoxLayout(tab_4);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        frame_6 = new QFrame(tab_4);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame_6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_9 = new QLabel(frame_6);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout_7->addWidget(label_9);

        tableWidget_5 = new QTableWidget(frame_6);
        if (tableWidget_5->columnCount() < 3)
            tableWidget_5->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(1, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(2, __qtablewidgetitem16);
        tableWidget_5->setObjectName(QStringLiteral("tableWidget_5"));
        tableWidget_5->setAlternatingRowColors(true);
        tableWidget_5->horizontalHeader()->setDefaultSectionSize(300);
        tableWidget_5->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_5->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_7->addWidget(tableWidget_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        pushButton_9 = new QPushButton(frame_6);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        horizontalLayout_9->addWidget(pushButton_9);

        pushButton_8 = new QPushButton(frame_6);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        horizontalLayout_9->addWidget(pushButton_8);


        verticalLayout_7->addLayout(horizontalLayout_9);


        verticalLayout_6->addWidget(frame_6);

        frame_7 = new QFrame(tab_4);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_8 = new QVBoxLayout(frame_7);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_10 = new QLabel(frame_7);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_8->addWidget(label_10);

        tableWidget_4 = new QTableWidget(frame_7);
        if (tableWidget_4->columnCount() < 3)
            tableWidget_4->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(2, __qtablewidgetitem19);
        tableWidget_4->setObjectName(QStringLiteral("tableWidget_4"));
        tableWidget_4->setAlternatingRowColors(true);
        tableWidget_4->horizontalHeader()->setDefaultSectionSize(300);
        tableWidget_4->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_4->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_8->addWidget(tableWidget_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        pushButton_11 = new QPushButton(frame_7);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        horizontalLayout_10->addWidget(pushButton_11);

        pushButton_10 = new QPushButton(frame_7);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));

        horizontalLayout_10->addWidget(pushButton_10);


        verticalLayout_8->addLayout(horizontalLayout_10);


        verticalLayout_6->addWidget(frame_7);

        tabWidget->addTab(tab_4, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalLayout_11 = new QHBoxLayout(tab_3);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        frame_8 = new QFrame(tab_3);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_9 = new QVBoxLayout(frame_8);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        label_11 = new QLabel(frame_8);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_9->addWidget(label_11);

        tableWidget_6 = new QTableWidget(frame_8);
        if (tableWidget_6->columnCount() < 3)
            tableWidget_6->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_6->setHorizontalHeaderItem(0, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_6->setHorizontalHeaderItem(1, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget_6->setHorizontalHeaderItem(2, __qtablewidgetitem22);
        tableWidget_6->setObjectName(QStringLiteral("tableWidget_6"));
        tableWidget_6->setAlternatingRowColors(true);
        tableWidget_6->horizontalHeader()->setDefaultSectionSize(150);
        tableWidget_6->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_9->addWidget(tableWidget_6);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        pushButton_13 = new QPushButton(frame_8);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));

        horizontalLayout_12->addWidget(pushButton_13);

        pushButton_12 = new QPushButton(frame_8);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        horizontalLayout_12->addWidget(pushButton_12);


        verticalLayout_9->addLayout(horizontalLayout_12);


        horizontalLayout_11->addWidget(frame_8);

        frame_9 = new QFrame(tab_3);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        verticalLayout_10 = new QVBoxLayout(frame_9);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        label_12 = new QLabel(frame_9);
        label_12->setObjectName(QStringLiteral("label_12"));

        verticalLayout_10->addWidget(label_12);

        tableWidget_7 = new QTableWidget(frame_9);
        if (tableWidget_7->columnCount() < 3)
            tableWidget_7->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(0, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(1, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(2, __qtablewidgetitem25);
        tableWidget_7->setObjectName(QStringLiteral("tableWidget_7"));
        tableWidget_7->setAlternatingRowColors(true);
        tableWidget_7->horizontalHeader()->setDefaultSectionSize(150);
        tableWidget_7->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_10->addWidget(tableWidget_7);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        pushButton_15 = new QPushButton(frame_9);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));

        horizontalLayout_13->addWidget(pushButton_15);

        pushButton_14 = new QPushButton(frame_9);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));

        horizontalLayout_13->addWidget(pushButton_14);


        verticalLayout_10->addLayout(horizontalLayout_13);


        horizontalLayout_11->addWidget(frame_9);

        tabWidget->addTab(tab_3, QString());

        horizontalLayout_3->addWidget(tabWidget);

        stackedWidget->addWidget(pageGeneric);
        pageDSA5 = new QWidget();
        pageDSA5->setObjectName(QStringLiteral("pageDSA5"));
        stackedWidget->addWidget(pageDSA5);

        horizontalLayout->addWidget(stackedWidget);

        CharEditor->setCentralWidget(centralwidget);

        retranslateUi(CharEditor);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(CharEditor);
    } // setupUi

    void retranslateUi(QMainWindow *CharEditor)
    {
        CharEditor->setWindowTitle(QApplication::translate("CharEditor", "MainWindow", Q_NULLPTR));
        label_3->setText(QApplication::translate("CharEditor", "System", Q_NULLPTR));
        systemComboBox->clear();
        systemComboBox->insertItems(0, QStringList()
         << QApplication::translate("CharEditor", "Generic", Q_NULLPTR)
         << QApplication::translate("CharEditor", "DSA5", Q_NULLPTR)
        );
        label->setText(QApplication::translate("CharEditor", "Display Name", Q_NULLPTR));
        label_2->setText(QApplication::translate("CharEditor", "Player", Q_NULLPTR));
        label_4->setText(QApplication::translate("CharEditor", "Character Icon", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("CharEditor", "...", Q_NULLPTR));
        saveButton->setText(QApplication::translate("CharEditor", "Save", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("CharEditor", "Cancel", Q_NULLPTR));
        label_5->setText(QApplication::translate("CharEditor", "General Character Information", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = generalInfoTable->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CharEditor", "Name: ", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = generalInfoTable->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CharEditor", "Surname: ", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = generalInfoTable->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CharEditor", "Species: ", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = generalInfoTable->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CharEditor", "Profession: ", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = generalInfoTable->verticalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CharEditor", "Culture: ", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = generalInfoTable->verticalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CharEditor", "Height: ", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = generalInfoTable->verticalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CharEditor", "Weight: ", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CharEditor", "General Information", Q_NULLPTR));
        tabWidget->setTabToolTip(tabWidget->indexOf(tab), QApplication::translate("CharEditor", "Contains general information about the character like his name, profession...", Q_NULLPTR));
        label_6->setText(QApplication::translate("CharEditor", "Skills 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = skillsTable1_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("CharEditor", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = skillsTable1_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QApplication::translate("CharEditor", "Value", Q_NULLPTR));
        addRowSkillsTable1_generic->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        removeRowSkillsTable1_generic->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        label_7->setText(QApplication::translate("CharEditor", "Skills 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = skillsTable2_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("CharEditor", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = skillsTable2_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("CharEditor", "Value", Q_NULLPTR));
        addRowSkillsTable2_generic->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        removeRowSkillsTable2_generic->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        label_8->setText(QApplication::translate("CharEditor", "Skills 3", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = skillsTable3_generic->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QApplication::translate("CharEditor", "Skill", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = skillsTable3_generic->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QApplication::translate("CharEditor", "Value", Q_NULLPTR));
        addRowSkillsTable3_generic->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        removeRowSkillsTable3_generic->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CharEditor", "Skills", Q_NULLPTR));
        label_9->setText(QApplication::translate("CharEditor", "Weapons", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_5->horizontalHeaderItem(0);
        ___qtablewidgetitem13->setText(QApplication::translate("CharEditor", "Name", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_5->horizontalHeaderItem(1);
        ___qtablewidgetitem14->setText(QApplication::translate("CharEditor", "Type", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_5->horizontalHeaderItem(2);
        ___qtablewidgetitem15->setText(QApplication::translate("CharEditor", "Damage", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        label_10->setText(QApplication::translate("CharEditor", "Armor", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_4->horizontalHeaderItem(0);
        ___qtablewidgetitem16->setText(QApplication::translate("CharEditor", "Type", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_4->horizontalHeaderItem(1);
        ___qtablewidgetitem17->setText(QApplication::translate("CharEditor", "Protection", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_4->horizontalHeaderItem(2);
        ___qtablewidgetitem18->setText(QApplication::translate("CharEditor", "Modificators", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("CharEditor", "Fight", Q_NULLPTR));
        label_11->setText(QApplication::translate("CharEditor", "Inventory 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_6->horizontalHeaderItem(0);
        ___qtablewidgetitem19->setText(QApplication::translate("CharEditor", "Item", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_6->horizontalHeaderItem(1);
        ___qtablewidgetitem20->setText(QApplication::translate("CharEditor", "Where", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget_6->horizontalHeaderItem(2);
        ___qtablewidgetitem21->setText(QApplication::translate("CharEditor", "Weight", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        label_12->setText(QApplication::translate("CharEditor", "Inventory 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget_7->horizontalHeaderItem(0);
        ___qtablewidgetitem22->setText(QApplication::translate("CharEditor", "Item", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget_7->horizontalHeaderItem(1);
        ___qtablewidgetitem23->setText(QApplication::translate("CharEditor", "Where", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget_7->horizontalHeaderItem(2);
        ___qtablewidgetitem24->setText(QApplication::translate("CharEditor", "Weight", Q_NULLPTR));
        pushButton_15->setText(QApplication::translate("CharEditor", "Add Row", Q_NULLPTR));
        pushButton_14->setText(QApplication::translate("CharEditor", "Remove Row", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("CharEditor", "Inventory", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CharEditor: public Ui_CharEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAREDITOR_H
