#include "characters.h"
#include "mainwindow.h"
#include "flowlayout.h"
#include "characterpage.h"

#include <QDir>
#include <QListWidget>
#include <QScrollArea>
#include <QSettings>
#include <QDebug>

QStringList getCharacterList(){
    QString folderPath = QDir::currentPath()+"/characters";
    QStringList files = getFiles(folderPath);
    QStringList characterFileNames;
//    for (QString file : files){
//        if (file.contains(".txt")){
//            characterFileNames.push_back(file);
//        }
//    }
    for (QString file : files){
        if (file.contains(".ini")){
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

QWidget* getCharacterPage(QString character){
    CharacterPage* charPage = new CharacterPage;

    QSettings charSettings("characters/"+character, QSettings::IniFormat);

    QString version = charSettings.value("Version", "SHOULD NOT BE VISIBLE").toString();
    QString name = charSettings.value("Name", "SHOULD NOT BE VISIBLE").toString();
    QString player = charSettings.value("Player", "SHOULD NOT BE VISIBLE").toString();
    int systemID = charSettings.value("System", 0).toInt();
    charPage->systemID = systemID;

    QString iconPath = charSettings.value("Icon", "SHOULD NOT BE VISIBLE").toString();

    qDebug() << character;
    qDebug() << name;
    qDebug() << player;
    qDebug() << "SystemID: " << systemID;

    charPage->setToolTip(name+" ("+player+")");
    charPage->setAccessibleName(iconPath);

    switch (systemID) {
    case 0:{
        // General Character Info
        QList<TableContent>* generalInfo_generic = new QList<TableContent>;
        int size = charSettings.beginReadArray("generalInfos");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            generalInfo_generic->push_back(content);
        }
        charPage->generalInfo_generic = generalInfo_generic;
        charSettings.endArray();

        // Skills
        QList<TableContent>* skills1_generic = new QList<TableContent>;
        size = charSettings.beginReadArray("skills1");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            skills1_generic->push_back(content);
        }
        charPage->skills1_generic = skills1_generic;
        charSettings.endArray();

        QList<TableContent>* skills2_generic = new QList<TableContent>;
        size = charSettings.beginReadArray("skills2");
        qDebug() << size;
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            skills2_generic->push_back(content);
        }
        charPage->skills2_generic = skills2_generic;
        charSettings.endArray();

        QList<TableContent>* skills3_generic = new QList<TableContent>;
        size = charSettings.beginReadArray("skills3");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            skills3_generic->push_back(content);
        }
        charPage->skills3_generic = skills3_generic;
        charSettings.endArray();

        // Weapons
        QList<TableContent3C>* weapons_generic = new QList<TableContent3C>;
        size = charSettings.beginReadArray("weapons");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent3C content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.midEntry = charSettings.value("midEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            weapons_generic->push_back(content);
        }
        charPage->weapons_generic = weapons_generic;
        charSettings.endArray();

        // Armor
        QList<TableContent3C>* armor_generic = new QList<TableContent3C>;
        size = charSettings.beginReadArray("armor");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent3C content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.midEntry = charSettings.value("midEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            armor_generic->push_back(content);
        }
        charPage->armor_generic = armor_generic;
        charSettings.endArray();

        // Inventory
        QList<TableContent3C>* inv1_generic = new QList<TableContent3C>;
        size = charSettings.beginReadArray("inventory1");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent3C content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.midEntry = charSettings.value("midEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            inv1_generic->push_back(content);
        }
        charPage->inv1_generic = inv1_generic;
        charSettings.endArray();

        QList<TableContent3C>* inv2_generic = new QList<TableContent3C>;
        size = charSettings.beginReadArray("inventory2");
        for (int i = 0; i<size; i++){
            charSettings.setArrayIndex(i);
            TableContent3C content;
            content.leftEntry = charSettings.value("leftEntry").toString();
            content.midEntry = charSettings.value("midEntry").toString();
            content.rightEntry = charSettings.value("rightEntry").toString();
            inv2_generic->push_back(content);
        }
        charPage->inv2_generic = inv2_generic;
        charSettings.endArray();

        break;
    }
    case 1:


        break;
    default:
        break;
    }

    charPage->updateUi();
    return charPage;
}
