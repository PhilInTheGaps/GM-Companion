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

    for (QString file : files){
        if (file.contains(".ini")){
            characterFileNames.push_back(file);
        }
    }
    qDebug() << "Character Files: " << characterFileNames.size();
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

QList<QStringList>* writeList(QString character, QString indicator, int columns){
    QSettings settings("characters/"+character, QSettings::IniFormat);
    QList<QStringList>* list = new QList<QStringList>;
    int size = settings.beginReadArray(indicator);
    for (int row = 0; row<size; row++){
        settings.setArrayIndex(row);
        QStringList subList;
        for (int column = 0; column<columns; column++){

            subList.push_back(settings.value("entry"+QString::number(column+1)).toString());
        }
        list->push_back(subList);
    }
    settings.endArray();
    return list;
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

    charPage->name = name;
    charPage->player = player;
    charPage->setToolTip(name+" ("+player+")");
    charPage->setAccessibleName(iconPath+","+QString::number(systemID));

    switch (systemID) {
    case 0:{
        // General Character Info
        charPage->generalInfo_generic = writeList(character, "generalInfos", 1);

        // Skills
        charPage->skills1_generic = writeList(character, "skills1", 2);
        charPage->skills2_generic = writeList(character, "skills2", 2);
        charPage->skills3_generic = writeList(character, "skills3", 2);

        // Weapons
        charPage->weapons_generic = writeList(character, "weapons", 3);

        // Armor
        charPage->armor_generic = writeList(character, "armor", 3);

        // Inventory
        charPage->inv1_generic = writeList(character, "inventory1", 2);
        charPage->inv2_generic = writeList(character, "inventory2", 2);

        break;
    }
    case 1:{
        // Persönliche Daten
        charPage->persDaten1_dsa5 = writeList(character, "persDaten1", 1);
        charPage->persDaten2_dsa5 = writeList(character, "persDaten2", 1);

        // Eigenschaften
        charPage->eigenschaften_dsa5 = writeList(character, "eigenschaften", 8);

        // Vorteile
        charPage->vorteile_dsa5 = writeList(character, "vorteile", 1);

        // Nachteile
        charPage->nachteile_dsa5 = writeList(character, "nachteile", 1);

        // Sonderfertigkeiten
        charPage->sonderf_dsa5 = writeList(character, "sonderfertigkeiten", 1);

        // Allgemeine Werte
        charPage->allgemein_dsa5 = writeList(character, "allgemein", 4);

        // AP
        charPage->ap_dsa5 = writeList(character, "ap", 4);

        // Schicksalspunkte
        charPage->schicksalsp_dsa5 = writeList(character, "schicksalsp", 4);

        // Fertigkeiten
        charPage->fertigkeiten1_dsa5 = writeList(character, "fertigkeiten1", 6);
        charPage->fertigkeiten2_dsa5 = writeList(character, "fertigkeiten2", 6);

        // Sprachen
        charPage->sprachen_dsa5 = writeList(character, "sprachen", 1);

        // Schriften
        charPage->schriften_dsa5 = writeList(character, "schriften", 1);

        // Kampftechniken
        charPage->ktw_dsa5 = writeList(character, "kampftechniken", 5);

        // LEP
        charPage->lep_dsa5 = writeList(character, "lep", 2);

        // Nahkampfwaffen
        charPage->nahkampfwaffen_dsa5 = writeList(character, "nahkampfwaffen", 9);

        // Fernkampfwaffen
        charPage->fernkampfwaffen_dsa5 = writeList(character, "fernkampfwaffen", 8);

        // Rüstungen
        charPage->ruestungen_dsa5 = writeList(character, "ruestungen", 6);

        // Schild / Parierwaffe
        charPage->schild_dsa5 = writeList(character, "schild", 4);

        // Kampfsonderfertigkeiten
        charPage->kSonderf_dsa5 = writeList(character, "kampfsonderfertigkeiten", 1);

        // Ausrüstung
        charPage->ausruestung1_dsa5 = writeList(character, "ausruestung1", 3);
        charPage->ausruestung2_dsa5 = writeList(character, "ausruestung2", 3);

        // Geldbeutel
        charPage->geld_dsa5 = writeList(character, "geldbeutel", 1);

        // Tier
        charPage->tierAllgemein_dsa5 = writeList(character, "tierAllgemein", 1);

        // Tier Angriff
        charPage->tierAngriff_dsa5 = writeList(character, "tierAngriff", 5);

        // Tier Aktionen
        charPage->tierAktionen_dsa5 = writeList(character, "tierAktionen", 1);

        // Tier Sonderf
        charPage->tierSonderf_dsa5 = writeList(character, "tierSonderfertigkeiten", 1);

        // Zauber
        charPage->zauber_dsa5 = writeList(character, "zauber", 11);

        // Zaubertricks
        charPage->zaubertricks_dsa5 = writeList(character, "zaubertricks", 1);

        // Magische Sonderfertigkeiten
        charPage->magischeSonderfertigkeiten_dsa5 = writeList(character, "magischeSonderfertigkeiten", 1);

        // Leiteigenschaft / Merkmal
        charPage->leiteigenschaftMerkmal_dsa5 = writeList(character, "leiteigenschaftMerkmal", 2);

        // Magische Tradition
        charPage->mTradition_dsa5 = writeList(character, "mTradition", 1);

        // Liturgien
        charPage->liturgien_dsa5 = writeList(character, "liturgien", 11);

        // Segnungen
        charPage->segnungen_dsa5 = writeList(character, "segnungen", 1);

        // Klerikale Sonderfertigkeiten
        charPage->klerikaleSonderfertigkeiten_dsa5 = writeList(character, "klerikaleSonderfertigkeiten", 1);

        // Leiteigenschaft / Aspekt
        charPage->leiteigenschaftAspekt_dsa5 = writeList(character, "leiteigenschaftAspekt", 2);

        // Klerikale Tradition
        charPage->kTradition_dsa5 = writeList(character, "kTradition", 1);

        break;
    }
    default:
        break;
    }

    charPage->updateUi();
    return charPage;
}