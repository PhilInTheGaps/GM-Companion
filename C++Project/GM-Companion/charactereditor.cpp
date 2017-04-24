#include "charactereditor.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "flowlayout.h"
#include <QLabel>
#include <QFile>
#include <QPushButton>
#include <QDir>
#include <QTextStream>

CharacterEditor::CharacterEditor(QObject *parent) : QObject(parent)
{

}

void CharacterEditor::open(){
    mainFrame = new QFrame;
    mainLayout = new QHBoxLayout;
    mainFrame->setLayout(mainLayout);

    mainFrame->setWindowTitle("Character Editor");
    mainFrame->setMinimumSize(1280, 720);

    QFrame* optionsFrame = new QFrame;
    optionsFrame->setFrameShape(QFrame::Box);
    optionsFrame->setMaximumWidth(250);
    QVBoxLayout* optionsLayout = new QVBoxLayout;
    optionsFrame->setLayout(optionsLayout);
    mainLayout->addWidget(optionsFrame);

    QLabel* optionsLabel = new QLabel;
    optionsLabel->setText("Options");
    optionsLayout->addWidget(optionsLabel);

    systemComboBox = new QComboBox;
    systemComboBox->addItems({"Generic", "DSA5"});
    optionsLayout->addWidget(systemComboBox);
    connect(systemComboBox, SIGNAL(currentIndexChanged(int)), SLOT(systemChanged(int)));

    QLabel* displayNameLabel = new QLabel;
    displayNameLabel->setText("Display Name:");
    optionsLayout->addWidget(displayNameLabel);

    displayNameEdit = new QLineEdit;
    optionsLayout->addWidget(displayNameEdit);

    QLabel* playerLabel = new QLabel;
    playerLabel->setText("Player:");
    optionsLayout->addWidget(playerLabel);

    playerEdit = new QLineEdit;
    optionsLayout->addWidget(playerEdit);

    optionsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding));

    QPushButton* saveButton = new QPushButton;
    saveButton->setText("Save Character");
    optionsLayout->addWidget(saveButton);
    connect(saveButton, SIGNAL(clicked(bool)), SLOT(save()));

    QPushButton* cancelButton = new QPushButton;
    cancelButton->setText("Cancel");
    optionsLayout->addWidget(cancelButton);
    connect(cancelButton, SIGNAL(clicked(bool)), SLOT(cancel()));

    stackedWidget = new QStackedWidget;
    stackedWidget->setFrameShape(QFrame::Box);
    mainLayout->addWidget(stackedWidget);

    // For unspecified characters
    QFrame* unspecificFrame = new QFrame;
    stackedWidget->addWidget(unspecificFrame);
    FlowLayout* unspecificLayout = new FlowLayout;
    unspecificFrame->setLayout(unspecificLayout);

    // General Character Info
    QFrame* generalFrame_u = new QFrame;
    generalFrame_u->setMinimumHeight(300);
    generalFrame_u->setFrameShape(QFrame::Box);
    unspecificLayout->addWidget(generalFrame_u);
    QVBoxLayout* generalLayout_u = new QVBoxLayout;
    generalFrame_u->setLayout(generalLayout_u);

    QLabel* generalLabel_u = new QLabel;
    generalLabel_u->setText("General Character Info");
    generalLayout_u->addWidget(generalLabel_u);

    generalTableWidget_u = new QTableWidget;
    generalTableWidget_u->setColumnCount(2);
    generalTableWidget_u->horizontalHeader()->hide();
    for (int i = 0; i<generalInfoSettings_u.size(); i++){
        generalTableWidget_u->insertRow(i);
        QTableWidgetItem* i1 = new QTableWidgetItem;
        i1->setFlags(i1->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem* i2 = new QTableWidgetItem;

        i1->setText(generalInfoSettings_u.at(i));

        generalTableWidget_u->setItem(i, 0, i1);
        generalTableWidget_u->setItem(i, 1, i2);
    }
    generalLayout_u->addWidget(generalTableWidget_u);

    // Skills
    QFrame* skillsFrame_u = new QFrame;
    //skillsFrame_u->setMinimumHeight(500);
    skillsFrame_u->setFrameShape(QFrame::Box);
    unspecificLayout->addWidget(skillsFrame_u);
    QVBoxLayout* skillsLayout_u = new QVBoxLayout;
    skillsFrame_u->setLayout(skillsLayout_u);

    QLabel* skillsLabel_u = new QLabel;
    skillsLabel_u->setText("Skills");
    skillsLayout_u->addWidget(skillsLabel_u);

    QFrame* skillsTables = new QFrame;
    QHBoxLayout* skillsTablesLayout = new QHBoxLayout;
    skillsTables->setLayout(skillsTablesLayout);
    skillsLayout_u->addWidget(skillsTables);

    skills1TableWidget_u = new QTableWidget;
    skills1TableWidget_u->setColumnCount(3);
    skills1TableWidget_u->setHorizontalHeaderLabels({"Skill", ": ", "Value"});
    skills1TableWidget_u->setColumnWidth(1, 10);
    skillsTablesLayout->addWidget(skills1TableWidget_u);

    skills2TableWidget_u = new QTableWidget;
    skills2TableWidget_u->setColumnCount(3);
    skills2TableWidget_u->setHorizontalHeaderLabels({"Skill", ": ", "Value"});
    skills2TableWidget_u->setColumnWidth(1, 10);
    skillsTablesLayout->addWidget(skills2TableWidget_u);

    skills3TableWidget_u = new QTableWidget;
    skills3TableWidget_u->setColumnCount(3);
    skills3TableWidget_u->setHorizontalHeaderLabels({"Skill", ": ", "Value"});
    skills3TableWidget_u->setColumnWidth(1, 10);
    skillsTablesLayout->addWidget(skills3TableWidget_u);

    QFrame* skillsButtonFrame = new QFrame;
    QHBoxLayout* skillsButtonLayout = new QHBoxLayout;
    skillsButtonFrame->setLayout(skillsButtonLayout);
    skillsLayout_u->addWidget(skillsButtonFrame);

    QPushButton* skills1AddSkillButton = new QPushButton;
    skills1AddSkillButton->setText("Add Row");
    skillsButtonLayout->addWidget(skills1AddSkillButton);
    connect(skills1AddSkillButton, SIGNAL(clicked(bool)), SLOT(skills1AddSkill_u()));

    QPushButton* skills1RemoveSkillButton = new QPushButton;
    skills1RemoveSkillButton->setText("Remove Row");
    skillsButtonLayout->addWidget(skills1RemoveSkillButton);
    connect(skills1RemoveSkillButton, SIGNAL(clicked(bool)), SLOT(skills1RemoveSkill_u()));

    QPushButton* skills2AddSkillButton = new QPushButton;
    skills2AddSkillButton->setText("Add Row");
    skillsButtonLayout->addWidget(skills2AddSkillButton);
    connect(skills2AddSkillButton, SIGNAL(clicked(bool)), SLOT(skills2AddSkill_u()));

    QPushButton* skills2RemoveSkillButton = new QPushButton;
    skills2RemoveSkillButton->setText("Remove Row");
    skillsButtonLayout->addWidget(skills2RemoveSkillButton);
    connect(skills2RemoveSkillButton, SIGNAL(clicked(bool)), SLOT(skills2RemoveSkill_u()));

    QPushButton* skills3AddSkillButton = new QPushButton;
    skills3AddSkillButton->setText("Add Row");
    skillsButtonLayout->addWidget(skills3AddSkillButton);
    connect(skills3AddSkillButton, SIGNAL(clicked(bool)), SLOT(skills3AddSkill_u()));

    QPushButton* skills3RemoveSkillButton = new QPushButton;
    skills3RemoveSkillButton->setText("Remove Row");
    skillsButtonLayout->addWidget(skills3RemoveSkillButton);
    connect(skills3RemoveSkillButton, SIGNAL(clicked(bool)), SLOT(skills3RemoveSkill_u()));

    // Inventory
    QFrame* inventoryFrame_u = new QFrame;
    QVBoxLayout* inventoryLayout_u = new QVBoxLayout;
    inventoryFrame_u->setLayout(inventoryLayout_u);
    inventoryFrame_u->setFrameShape(QFrame::Box);
    unspecificLayout->addWidget(inventoryFrame_u);

    QLabel* inventoryLabel_u = new QLabel;
    inventoryLabel_u->setText("Inventory");
    inventoryLayout_u->addWidget(inventoryLabel_u);

    QFrame* inventoryTableFrame_u = new QFrame;
    QHBoxLayout* inventoryTableLayout_u = new QHBoxLayout;
    inventoryTableFrame_u->setLayout(inventoryTableLayout_u);
    inventoryLayout_u->addWidget(inventoryTableFrame_u);

    inventory1ListWidget_u = new QListWidget;
    inventory1ListWidget_u->setStyleSheet("QListView::item { background-color: #888888;}");
    inventoryTableLayout_u->addWidget(inventory1ListWidget_u);

    inventory2ListWidget_u = new QListWidget;
    inventory2ListWidget_u->setStyleSheet("QListView::item { background-color: #888888;}");
    inventoryTableLayout_u->addWidget(inventory2ListWidget_u);

    QFrame* inventoryButtonFrame_u = new QFrame;
    QHBoxLayout* inventoryButtonLayout_u = new QHBoxLayout;
    inventoryButtonFrame_u->setLayout(inventoryButtonLayout_u);
    inventoryLayout_u->addWidget(inventoryButtonFrame_u);

    QPushButton* addRowInv1Button_u = new QPushButton;
    addRowInv1Button_u->setText("Add Row");
    inventoryButtonLayout_u->addWidget(addRowInv1Button_u);
    connect(addRowInv1Button_u, SIGNAL(clicked(bool)), SLOT(inv1AddRow_u()));

    QPushButton* remRowInv1Button_u = new QPushButton;
    remRowInv1Button_u->setText("Add Row");
    inventoryButtonLayout_u->addWidget(remRowInv1Button_u);
    connect(remRowInv1Button_u, SIGNAL(clicked(bool)), SLOT(inv1RemRow_u()));

    QPushButton* addRowInv2Button_u = new QPushButton;
    addRowInv2Button_u->setText("Add Row");
    inventoryButtonLayout_u->addWidget(addRowInv2Button_u);
    connect(addRowInv2Button_u, SIGNAL(clicked(bool)), SLOT(inv2AddRow_u()));

    QPushButton* remRowInv2Button_u = new QPushButton;
    remRowInv2Button_u->setText("Add Row");
    inventoryButtonLayout_u->addWidget(remRowInv2Button_u);
    connect(remRowInv2Button_u, SIGNAL(clicked(bool)), SLOT(inv2RemRow_u()));


    // For DSA5 Characters
    QFrame* dsa5Frame = new QFrame;
    stackedWidget->addWidget(dsa5Frame);

    //mainFrame->show();
    mainFrame->showMaximized();
}

void CharacterEditor::systemChanged(int index){
    stackedWidget->setCurrentIndex(index);
}

void CharacterEditor::save(){
    QString outputFilename = displayNameEdit->text().toLower()+".txt";
    QFile outputFile(QDir::currentPath()+"/characters/"+outputFilename);
    outputFile.open(QIODevice::WriteOnly);

    QTextStream outStream(&outputFile);

    outStream << "NAME=" << displayNameEdit->text() << ";\n";
    outStream << "PLAYER=" << playerEdit->text() << ";\n";
    outStream << "SYSTEM=" << systemComboBox->currentText() << ";\n";

    switch (systemComboBox->currentIndex()) {
    case 0:
        outStream << "GENERAL_CHARACTER_INFO={";
        for (int i = 0; i<generalInfoSettings_u.size(); i++){
            outStream << generalTableWidget_u->item(i, 0)->text();
            outStream << generalTableWidget_u->item(i, 1)->text();
            if (i != generalInfoSettings_u.size()-1){
                outStream << ",";
            }
        }
        outStream << "};\n";

        outStream << "SKILLS1={";
        for (int i = 0; i<skills1TableWidget_u->rowCount(); i++){
            outStream << skills1TableWidget_u->item(i, 0)->text();
            outStream << skills1TableWidget_u->item(i, 1)->text();
            outStream << skills1TableWidget_u->item(i, 2)->text();
            if (i != skills1TableWidget_u->rowCount()-1){
                outStream << ",";
            }
        }
        outStream << "};\n";

        outStream << "SKILLS2={";
        for (int i = 0; i<skills2TableWidget_u->rowCount(); i++){
            outStream << skills2TableWidget_u->item(i, 0)->text();
            outStream << skills2TableWidget_u->item(i, 1)->text();
            outStream << skills2TableWidget_u->item(i, 2)->text();
            if (i != skills2TableWidget_u->rowCount()-1){
                outStream << ",";
            }
        }
        outStream << "};\n";

        outStream << "SKILLS3={";
        for (int i = 0; i<skills3TableWidget_u->rowCount(); i++){
            outStream << skills3TableWidget_u->item(i, 0)->text();
            outStream << skills3TableWidget_u->item(i, 1)->text();
            outStream << skills3TableWidget_u->item(i, 2)->text();
            if (i != skills3TableWidget_u->rowCount()-1){
                outStream << ",";
            }
        }
        outStream << "};\n";

        outStream << "INVENTORY1={";
        for (int i = 0; i<inventory1ListWidget_u->count(); i++){
            outStream << inventory1ListWidget_u->item(i)->text();
            if (i != inventory1ListWidget_u->count()-1){
                outStream << ",";
            }
        }
        outStream << "};\n";

        outStream << "INVENTORY2={";
        for (int i = 0; i<inventory2ListWidget_u->count(); i++){
            outStream << inventory2ListWidget_u->item(i)->text();
            if (i != inventory2ListWidget_u->count()-1){
                outStream << ",";
            }
        }
        outStream << "};\n";

        outStream << "WEAPONS={};";
        outStream << "ARMOR={};";

        break;
    case 1:

        outStream << "GENERAL_CHARACTER_INFO={<noch nicht implementiert>};";
        outStream << "WEAPONS={<noch nicht implementiert>};";
        outStream << "ARMOR={<noch nicht implementiert>};";
        outStream << "KAMPFSONDERFERTIGKEITEN={<noch nicht implementiert>};";
        outStream << "KAMPFTECHNIKEN={<noch nicht implementiert>};";
        outStream << "EIGENSCHAFTEN={<noch nicht implementiert>};";
        outStream << "AP={<noch nicht implementiert>};";
        outStream << "VORTEILE={<noch nicht implementiert>};";
        outStream << "NACHTEILE={<noch nicht implementiert>};";
        outStream << "ALLGEMEINE_SONDERFERTIGKEITEN={<noch nicht implementiert>};";
        outStream << "GRUNDWERTE={<noch nicht implementiert>};";
        outStream << "SPRACHEN={<noch nicht implementiert>};";
        outStream << "SCHRIFTEN={<noch nicht implementiert>};";
        outStream << "SKILLS1={<noch nicht implementiert>};";
        outStream << "SKILLS2={<noch nicht implementiert>};";
        outStream << "SKILLS3={<noch nicht implementiert>};";
        outStream << "INVENTORY1={<noch nicht implementiert>};";
        outStream << "INVENTORY2={<noch nicht implementiert>};";

        break;
    }

    outputFile.close();
    mainFrame->close();
}

void CharacterEditor::cancel(){
    mainFrame->close();
}

void CharacterEditor::skills1AddSkill_u(){
    skills1TableWidget_u->insertRow(skills1TableWidget_u->rowCount());
    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(": ");
    skills1TableWidget_u->setItem(skills1TableWidget_u->rowCount()-1, 1, i);
}

void CharacterEditor::skills2AddSkill_u(){
    skills2TableWidget_u->insertRow(skills2TableWidget_u->rowCount());
    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(": ");
    skills2TableWidget_u->setItem(skills2TableWidget_u->rowCount()-1, 1, i);
}

void CharacterEditor::skills3AddSkill_u(){
    skills3TableWidget_u->insertRow(skills3TableWidget_u->rowCount());
    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(": ");
    skills3TableWidget_u->setItem(skills3TableWidget_u->rowCount()-1, 1, i);
}

void CharacterEditor::skills1RemoveSkill_u(){
    if (skills1TableWidget_u->rowCount()>0){
        skills1TableWidget_u->setRowCount(skills1TableWidget_u->rowCount()-1);
    }
}

void CharacterEditor::skills2RemoveSkill_u(){
    if (skills2TableWidget_u->rowCount()>0){
        skills2TableWidget_u->setRowCount(skills2TableWidget_u->rowCount()-1);
    }
}

void CharacterEditor::skills3RemoveSkill_u(){
    if (skills3TableWidget_u->rowCount()>0){
        skills3TableWidget_u->setRowCount(skills3TableWidget_u->rowCount()-1);
    }
}

void CharacterEditor::inv1AddRow_u(){
    QListWidgetItem* item = new QListWidgetItem;
    item->setFlags (item->flags () | Qt::ItemIsEditable);
    inventory1ListWidget_u->addItem(item);
}

void CharacterEditor::inv2AddRow_u(){
    QListWidgetItem* item = new QListWidgetItem;
    item->setFlags (item->flags () | Qt::ItemIsEditable);
    inventory2ListWidget_u->addItem(item);
}

void CharacterEditor::inv1RemRow_u(){
    if (inventory1ListWidget_u->count()>0){
        QListWidgetItem* item = inventory1ListWidget_u->takeItem(inventory1ListWidget_u->count()-1);
        delete item;
    }
}

void CharacterEditor::inv2RemRow_u(){
    if (inventory2ListWidget_u->count()>0){
        QListWidgetItem* item = inventory2ListWidget_u->takeItem(inventory2ListWidget_u->count()-1);
        delete item;
    }
}
