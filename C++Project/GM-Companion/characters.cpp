#include "characters.h"
#include "mainwindow.h"
#include "flowlayout.h"
#include "charactereditor.h"

#include <QDir>
#include <QListWidget>
#include <QScrollArea>

QStringList getCharacterList(){
    QString folderPath = QDir::currentPath()+"/characters";
    QStringList files = getFiles(folderPath);
    QStringList characterFileNames;
    for (QString file : files){
        if (file.contains(".txt")){
            characterFileNames.push_back(file);
        }
    }
    return characterFileNames;
}

QFrame* createLabelFrame(QString first, QString second){
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout;
    frameLayout->setAlignment(Qt::AlignLeft);
    frame->setLayout(frameLayout);

    QLabel *label1 = new QLabel;
    label1->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    label1->setText(first);
    frameLayout->addWidget(label1);

    QLabel *label2 = new QLabel;
    label2->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    label2->setText(second);
    frameLayout->addWidget(label2);
    return frame;
}

QFrame* generateFrame(QString indicator, QString headline, QString characterFileContent, int maxWidth=210){
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::Box);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    frame->setMaximumWidth(maxWidth);
    QVBoxLayout *layout = new QVBoxLayout;
    frame->setLayout(layout);

    QLabel *label = new QLabel;
    label->setText(headline);
    layout->addWidget(label);

    QListWidget *listWidget = new QListWidget;
    layout->addWidget(listWidget);

    int index1 = characterFileContent.indexOf(indicator);
    int index2 = characterFileContent.indexOf("};", index1);
    QString string = characterFileContent.mid(index1+indicator.length(), index2-index1-indicator.length());

    QStringList list = string.split(",");

    for (QString item : list){
        listWidget->addItem(item);
    }

    return frame;
}

QFrame* getCharacterPage(QString character){
    QFile characterFile(QDir::currentPath()+"/characters/"+character);
    characterFile.open(QIODevice::ReadOnly);
    QString characterFileContent = characterFile.readAll();

    int index1 = 0;
    int index2 = 0;

    QFrame *baseFrame = new QFrame;
    QHBoxLayout *baseLayout = new QHBoxLayout;
    baseFrame->setLayout(baseLayout);

    QFrame *leftFrame = new QFrame;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftFrame->setLayout(leftLayout);
    baseLayout->addWidget(leftFrame);

    QLabel* infoLabel = new QLabel;
    infoLabel->setText("Info");
    leftLayout->addWidget(infoLabel);

    QFrame *mainFrame = new QFrame;
    mainFrame->setFrameShape(QFrame::Box);
    FlowLayout *mainLayout = new FlowLayout;
    mainFrame->setLayout(mainLayout);

    QScrollArea *mainScrollArea = new QScrollArea;
    mainScrollArea->setMinimumWidth(200);
    mainScrollArea->setWidget(mainFrame);
    mainScrollArea->setWidgetResizable(true);

    leftLayout->addWidget(mainScrollArea);

    index1 = characterFileContent.indexOf("NAME=");
    index2 = characterFileContent.indexOf(";", index1);
    QString name = characterFileContent.mid(index1+5, index2-index1-5);
    index1 = characterFileContent.indexOf("PLAYER=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString player = characterFileContent.mid(index1+7, index2-index1-7);
    baseFrame->setToolTip(name+ " ("+player+")");

    // Checks for which system the character was created
    index1 = characterFileContent.indexOf("SYSTEM=");
    index2 = characterFileContent.indexOf(";", index1);
    QString systemString = characterFileContent.mid(index1+7, index2-index1-7);
    System system;
    if (QString::compare(systemString, "DSA5") == 0){
        system = DSA5;
    }
    else{
        system = Unspecific;
    }

    // General Character Info
    QFrame* generalInfoFrame = generateFrame("GENERAL_CHARACTER_INFO={", "General Character Info", characterFileContent);
    mainLayout->addWidget(generalInfoFrame);

    // Fight Stats
    QLabel* fightLabel = new QLabel;
    fightLabel->setText("Fight");
    leftLayout->addWidget(fightLabel);

    QFrame *fightFrame = new QFrame;
    fightFrame->setFrameShape(QFrame::Box);
    FlowLayout *fightLayout = new FlowLayout;
    fightFrame->setLayout(fightLayout);

    QScrollArea *fightScrollArea = new QScrollArea;
    fightScrollArea->setMinimumWidth(200);
    fightScrollArea->setWidget(fightFrame);
    fightScrollArea->setWidgetResizable(true);

    leftLayout->addWidget(fightScrollArea);

    // Weapons
    QFrame* weaponFrame = generateFrame("WEAPONS={", "Weapons", characterFileContent, 400);
    fightLayout->addWidget(weaponFrame);

    // Armor
    QFrame* armorFrame = generateFrame("ARMOR={", "Armor", characterFileContent, 400);
    fightLayout->addWidget(armorFrame);

    // SYSTEM SPECIFIC INFORMATION
    switch (system) {
    case DSA5:{
        // Base Stats
        QFrame *eigenschaftenFrame = generateFrame("EIGENSCHAFTEN={", "Eigenschaften", characterFileContent);
        mainLayout->addWidget(eigenschaftenFrame);

        // AP
        QFrame *apFrame = generateFrame("AP={", "AP", characterFileContent);
        mainLayout->addWidget(apFrame);

        // Vorteile
        QFrame *vorteileFrame = generateFrame("VORTEILE={", "Vorteile", characterFileContent);
        mainLayout->addWidget(vorteileFrame);

        // Nachteile
        QFrame *nachteileFrame = generateFrame("NACHTEILE={", "Nachteile", characterFileContent);
        mainLayout->addWidget(nachteileFrame);

        // Allgemeine Sonderfertigkeiten
        QFrame *sonderfertigkeitenFrame = generateFrame("ALLGEMEINE_SONDERFERTIGKEITEN={", "Allgemeine Sonderfertigkeiten", characterFileContent);
        mainLayout->addWidget(sonderfertigkeitenFrame);

        // Grundwerte
        QFrame *grundwerteFrame = generateFrame("GRUNDWERTE={", "Grundwerte",characterFileContent);
        mainLayout->addWidget(grundwerteFrame);

        // Kampftechniken
        QFrame* ktFrame = generateFrame("KAMPFTECHNIKEN={", "Kampftechniken", characterFileContent);
        fightLayout->addWidget(ktFrame);

        // Kampfsonderfertigkeiten
        QFrame* ksfFrame = generateFrame("KAMPFSONDERFERTIGKEITEN={", "Kampfsonderfertigkeiten", characterFileContent);
        fightLayout->addWidget(ksfFrame);

        break;}
    default:
        break;
    }

    // Everything on the right side
    QFrame *rightFrame = new QFrame;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightFrame->setLayout(rightLayout);

    baseLayout->addWidget(rightFrame);

    // Skills
    QFrame *skillsFrame = new QFrame;
    QHBoxLayout *skillsLayout = new QHBoxLayout;
    skillsFrame->setLayout(skillsLayout);
    skillsFrame->setFrameShape(QFrame::Box);

    QLabel *skillsLabel = new QLabel;
    skillsLabel->setText("Skills");
    rightLayout->addWidget(skillsLabel);

    index1 = characterFileContent.indexOf("SKILLS1={");
    index2 = characterFileContent.indexOf("};", index1);
    QString skills1String = characterFileContent.mid(index1+9, index2-index1-9);

    QStringList skillsList1 = skills1String.split(",");

    QListWidget *skillsList1Widget = new QListWidget;
    skillsLayout->addWidget(skillsList1Widget);
    for (QString skill : skillsList1){
        skillsList1Widget->addItem(skill);
    }

    index1 = characterFileContent.indexOf("SKILLS2={");
    index2 = characterFileContent.indexOf("};", index1);
    QString skills2String = characterFileContent.mid(index1+9, index2-index1-9);

    QStringList skillsList2 = skills2String.split(",");

    QListWidget *skillsList2Widget = new QListWidget;
    skillsLayout->addWidget(skillsList2Widget);
    for (QString skill : skillsList2){
        skillsList2Widget->addItem(skill);
    }

    index1 = characterFileContent.indexOf("SKILLS3={");
    index2 = characterFileContent.indexOf("};", index1);
    QString skills3String = characterFileContent.mid(index1+9, index2-index1-9);

    QStringList skillsList3 = skills3String.split(",");

    QListWidget *skillsList3Widget = new QListWidget;
    skillsLayout->addWidget(skillsList3Widget);
    for (QString skill : skillsList3){
        skillsList3Widget->addItem(skill);
    }

    rightLayout->addWidget(skillsFrame);

    // Inventory
    QFrame *inventoryFrame = new QFrame;
    inventoryFrame->setFrameShape(QFrame::Box);
    QHBoxLayout *inventoryLayout = new QHBoxLayout;
    inventoryFrame->setLayout(inventoryLayout);

    QLabel *inventoryLabel = new QLabel;
    inventoryLabel->setText("Inventory");
    rightLayout->addWidget(inventoryLabel);

    index1 = characterFileContent.indexOf("INVENTORY1={");
    index2 = characterFileContent.indexOf("};", index1);
    QString inventory1String = characterFileContent.mid(index1+12, index2-index1-12);

    QStringList inventoryList1 = inventory1String.split(",");

    QListWidget *inventoryList1Widget = new QListWidget;
    inventoryLayout->addWidget(inventoryList1Widget);
    for (QString item : inventoryList1){
        inventoryList1Widget->addItem(item);
    }

    index1 = characterFileContent.indexOf("INVENTORY2={");
    index2 = characterFileContent.indexOf("};", index1);
    QString inventory2String = characterFileContent.mid(index1+12, index2-index1-12);

    QStringList inventoryList2 = inventory2String.split(",");

    QListWidget *inventoryList2Widget = new QListWidget;
    inventoryLayout->addWidget(inventoryList2Widget);
    for (QString item : inventoryList2){
        inventoryList2Widget->addItem(item);
    }

    rightLayout->addWidget(inventoryFrame);

    characterFile.close();
    return baseFrame;
}
