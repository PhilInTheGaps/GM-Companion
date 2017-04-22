#include "characters.h"
#include "mainwindow.h"
#include "flowlayout.h"

#include <QDir>
#include <QListWidget>
#include <QScrollArea>

QStringList getCharacterList(){
    QString folderPath = QDir::currentPath()+"/characters";
    QStringList characterFileNames = getFiles(folderPath);
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

QFrame* getCharacterPage(QString character){
    QFile characterFile(QDir::currentPath()+"/characters/"+character);
    characterFile.open(QIODevice::ReadOnly);
    QString characterFileContent = characterFile.readAll();

    int index1 = 0;
    int index2 = 0;

    QFrame *baseFrame = new QFrame;
    QHBoxLayout *baseLayout = new QHBoxLayout;
    baseFrame->setLayout(baseLayout);

    QFrame *mainFrame = new QFrame;
    mainFrame->setMinimumWidth(200);
    mainFrame->setFrameShape(QFrame::Box);
    FlowLayout *mainLayout = new FlowLayout;
    mainFrame->setLayout(mainLayout);

    QScrollArea *mainScrollArea = new QScrollArea;
    mainScrollArea->setWidgetResizable(true);
    mainScrollArea->setWidget(mainFrame);

    baseLayout->addWidget(mainScrollArea);

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
        system = Unknown;
    }

    QFrame *generalInfoFrame = new QFrame;
    generalInfoFrame->setFrameShape(QFrame::Box);
    mainLayout->addWidget(generalInfoFrame);
    QVBoxLayout *generalInfoFrameLayout = new QVBoxLayout;
    generalInfoFrame->setLayout(generalInfoFrameLayout);

    QLabel *generalInfoLabel = new QLabel;
    generalInfoLabel->setText("General Character Info");
    generalInfoFrameLayout->addWidget(generalInfoLabel);

    index1 = characterFileContent.indexOf("CONTENT");

    // Character Name
    index1 = characterFileContent.indexOf("NAME=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString characterName = characterFileContent.mid(index1+5, index2-index1-5);
    QFrame *nameFrame = createLabelFrame("Name: ", characterName);
    generalInfoFrameLayout->addWidget(nameFrame);

    // Character Surname
    index1 = characterFileContent.indexOf("SURNAME=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString characterSurname = characterFileContent.mid(index1+8, index2-index1-8);
    QFrame *surnameFrame = createLabelFrame("Surname: ", characterSurname);
    generalInfoFrameLayout->addWidget(surnameFrame);

    // Character Species
    index1 = characterFileContent.indexOf("SPECIES=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString species = characterFileContent.mid(index1+8, index2-index1-8);
    QFrame *speciesFrame = createLabelFrame("Species: ", species);
    generalInfoFrameLayout->addWidget(speciesFrame);

    // Character Profession
    index1 = characterFileContent.indexOf("PROFESSION=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString profession = characterFileContent.mid(index1+11, index2-index1-11);
    QFrame *professionFrame = createLabelFrame("Profession: ", profession);
    generalInfoFrameLayout->addWidget(professionFrame);

    // Character Culture
    index1 = characterFileContent.indexOf("CULTURE=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString culture = characterFileContent.mid(index1+8, index2-index1-8);
    QFrame *cultureFrame = createLabelFrame("Culture: ", culture);
    generalInfoFrameLayout->addWidget(cultureFrame);

    // Character Age
    index1 = characterFileContent.indexOf("AGE=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString age = characterFileContent.mid(index1+4, index2-index1-4);
    QFrame *ageFrame = createLabelFrame("Age: ", age);
    generalInfoFrameLayout->addWidget(ageFrame);

    // Character Size
    index1 = characterFileContent.indexOf("SIZE=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString size = characterFileContent.mid(index1+5, index2-index1-5);
    QFrame *sizeFrame = createLabelFrame("Size: ", size);
    generalInfoFrameLayout->addWidget(sizeFrame);

    // Character Weight
    index1 = characterFileContent.indexOf("WEIGHT=", index1);
    index2 = characterFileContent.indexOf(";", index1);
    QString weight = characterFileContent.mid(index1+7, index2-index1-7);
    QFrame *weightFrame = createLabelFrame("Weight: ", weight);
    generalInfoFrameLayout->addWidget(weightFrame);

    // SYSTEM SPECIFIC INFORMATION
    switch (system) {
    case DSA5:{
        // Base Stats
        QFrame *baseStatsFrame = new QFrame;
        baseStatsFrame->setFrameShape(QFrame::Box);
        QVBoxLayout *baseStatsFrameLayout = new QVBoxLayout;
        baseStatsFrame->setLayout(baseStatsFrameLayout);

        index1 = characterFileContent.indexOf("EIGENSCHAFTEN");

        QLabel *baseStatsLabel = new QLabel;
        baseStatsLabel->setText("Eigenschaften");
        baseStatsFrameLayout->addWidget(baseStatsLabel);

        QStringList indicators = {"MUT=", "KLUGHEIT=", "INTUITION=", "CHARISMA=",
                                  "FINGERFERTIGKEIT=", "GESCHICKLICHKEIT=", "KONSTITUTION=", "KOERPERKRAFT="};
        QStringList titles = {"Mut: ", "Klugheit: ", "Intuition: ", "Charisma: ",
                              "Fingerfertigkeit: ", "Geschicklichkeit: ", "Konstitution: ", "Koerperkraft: "};

        for (int i = 0; i<indicators.size(); i++){
            QString s1 = indicators.at(i);
            index1 = characterFileContent.indexOf(s1, index1);
            index2 = characterFileContent.indexOf(";", index1);
            QString s2 = characterFileContent.mid(index1+s1.length(), index2-index1-s1.length());
            QFrame *f = createLabelFrame(titles.at(i), s2);
            baseStatsFrameLayout->addWidget(f);
        }

        mainLayout->addWidget(baseStatsFrame);

        // AP
        QFrame *apFrame = new QFrame;
        apFrame->setFrameShape(QFrame::Box);
        QVBoxLayout *apFrameLayout = new QVBoxLayout;
        apFrame->setLayout(apFrameLayout);

        index1 = characterFileContent.indexOf("AP");

        QLabel *apLabel = new QLabel;
        apLabel->setText("Eigenschaften");
        apFrameLayout->addWidget(apLabel);

        QStringList apindicators = {"ERFAHRUNGSGRAD=", "GESAMT=", "GESAMMELT=", "AUSGEGEBEN="};
        QStringList aptitles = {"Erfahrungsgrad: ", "AP Gesamt: ", "AP Gesammelt: ", "AP Ausgegeben: "};

        for (int i = 0; i<apindicators.size(); i++){
            QString s1 = apindicators.at(i);
            index1 = characterFileContent.indexOf(s1, index1);
            index2 = characterFileContent.indexOf(";", index1);
            QString s2 = characterFileContent.mid(index1+s1.length(), index2-index1-s1.length());
            QFrame *f = createLabelFrame(aptitles.at(i), s2);
            apFrameLayout->addWidget(f);
        }

        mainLayout->addWidget(apFrame);

        // Vorteile
        QFrame *vorteileFrame = new QFrame;
        vorteileFrame->setFrameShape(QFrame::Box);
        QVBoxLayout *vorteileFrameLayout = new QVBoxLayout;
        vorteileFrame->setLayout(vorteileFrameLayout);

        QLabel *vorteileLabel = new QLabel;
        vorteileLabel->setText("Vorteile");
        vorteileFrameLayout->addWidget(vorteileLabel);

        QListWidget *vorteileListWidget = new QListWidget;
        vorteileFrameLayout->addWidget(vorteileListWidget);

        index1 = characterFileContent.indexOf("VORTEILE={");
        index2 = characterFileContent.indexOf("};", index1);
        QString vorteileString = characterFileContent.mid(index1+10, index2-index1-10);

        QStringList vorteileList = vorteileString.split(",");

        for (QString vorteil : vorteileList){
            vorteileListWidget->addItem(vorteil);
        }

        mainLayout->addWidget(vorteileFrame);

        // Nachteile
        QFrame *nachteileFrame = new QFrame;
        nachteileFrame->setFrameShape(QFrame::Box);
        QVBoxLayout *nachteileFrameLayout = new QVBoxLayout;
        nachteileFrame->setLayout(nachteileFrameLayout);

        QLabel *nachteileLabel = new QLabel;
        nachteileLabel->setText("Nachteile");
        nachteileFrameLayout->addWidget(nachteileLabel);

        QListWidget *nachteileListWidget = new QListWidget;
        nachteileFrameLayout->addWidget(nachteileListWidget);

        index1 = characterFileContent.indexOf("NACHTEILE={");
        index2 = characterFileContent.indexOf("};", index1);
        QString nachteileString = characterFileContent.mid(index1+11, index2-index1-11);

        QStringList nachteileList = nachteileString.split(",");

        for (QString nachteil : nachteileList){
            nachteileListWidget->addItem(nachteil);
        }

        mainLayout->addWidget(nachteileFrame);

        // Allgemeine Sonderfertigkeiten
        QFrame *sonderfertigkeitenFrame = new QFrame;
        sonderfertigkeitenFrame->setFrameShape(QFrame::Box);
        QVBoxLayout *sonderfertigkeitenFrameLayout = new QVBoxLayout;
        sonderfertigkeitenFrame->setLayout(sonderfertigkeitenFrameLayout);

        QLabel *sonderfertigkeitenLabel = new QLabel;
        sonderfertigkeitenLabel->setText("Allgemeine Sonderfertigkeiten");
        sonderfertigkeitenFrameLayout->addWidget(sonderfertigkeitenLabel);

        QListWidget *sonderfertigkeitenListWidget = new QListWidget;
        sonderfertigkeitenFrameLayout->addWidget(sonderfertigkeitenListWidget);

        index1 = characterFileContent.indexOf("ALLGEMEINE_SONDERFERTIGKEITEN={");
        index2 = characterFileContent.indexOf("};", index1);
        QString sonderfertigkeitenString = characterFileContent.mid(index1+31, index2-index1-31);

        QStringList sonderfertigkeitenList = sonderfertigkeitenString.split(",");

        for (QString sonderfertigkeit : sonderfertigkeitenList){
            sonderfertigkeitenListWidget->addItem(sonderfertigkeit);
        }

        mainLayout->addWidget(sonderfertigkeitenFrame);

        // Grundwerte
        QFrame *grundwerteFrame = new QFrame;
        grundwerteFrame->setFrameShape(QFrame::Box);
        QVBoxLayout *grundwerteFrameLayout = new QVBoxLayout;
        grundwerteFrame->setLayout(grundwerteFrameLayout);

        index1 = characterFileContent.indexOf("GRUNDWERTE");

        QLabel *grundwerteLabel = new QLabel;
        grundwerteLabel->setText("Grundwerte");
        grundwerteFrameLayout->addWidget(grundwerteLabel);

        QStringList grundwerteindicators = {"LEBENSENERGIE_MAX=", "ASTRALENERGIE_MAX=", "KARMAENERGIE_MAX=", "SEELENKRAFT_MAX=",
                                            "ZAEHIGKEIT_MAX=", "AUSWEICHEN_MAX=", "INITIATIVE_MAX=", "GESCHWINDIGKEIT_MAX=",
                                            "SCHICKSALSPUNKTE_AKTUELL=", "SCHICKSALSPUNKTE_MAX="};
        QStringList grundwertetitles = {"Lebensenergie (Max.): ", "Astralenergie (Max.): ", "Karmaenergie (Max.): ",
                                        "Seelenkraft (Max.): ", "Zaehigkeit (Max.): ", "Ausweichen (Max.): ",
                                        "Initiative (Max.): ", "Geschwindigkeit (Max.): ", "Schicksalspunkte (Aktuell): ",
                                        "Schicksalspunkte (Max.): "};

        for (int i = 0; i<grundwerteindicators.size(); i++){
            QString s1 = grundwerteindicators.at(i);
            index1 = characterFileContent.indexOf(s1, index1);
            index2 = characterFileContent.indexOf(";", index1);
            QString s2 = characterFileContent.mid(index1+s1.length(), index2-index1-s1.length());
            QFrame *f = createLabelFrame(grundwertetitles.at(i), s2);
            grundwerteFrameLayout->addWidget(f);
        }

        mainLayout->addWidget(grundwerteFrame);


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
