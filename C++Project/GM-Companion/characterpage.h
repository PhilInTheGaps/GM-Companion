#ifndef CHARACTERPAGE_H
#define CHARACTERPAGE_H

#include <QStackedWidget>
#include <QStringList>
#include "characters.h"
#include <QList>
#include <QTableWidget>

namespace Ui {
class CharacterPage;
}

class CharacterPage : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CharacterPage(QWidget *parent = 0);
    ~CharacterPage();

    void updateUi();

    int systemID = 0;

    // General
    QString name;
    QString player;

    // Generic
    QList<QStringList>* generalInfo_generic;

    QList<QStringList>* skills1_generic;
    QList<QStringList>* skills2_generic;
    QList<QStringList>* skills3_generic;

    QList<QStringList>* weapons_generic;
    QList<QStringList>* armor_generic;

    QList<QStringList>* inv1_generic;
    QList<QStringList>* inv2_generic;

    // DSA5
    QList<QStringList>* persDaten1_dsa5;
    QList<QStringList>* persDaten2_dsa5;

    QList<QStringList>* eigenschaften_dsa5;
    QList<QStringList>* vorteile_dsa5;
    QList<QStringList>* nachteile_dsa5;
    QList<QStringList>* sonderf_dsa5;

    QList<QStringList>* ap_dsa5;
    QList<QStringList>* allgemein_dsa5;
    QList<QStringList>* schicksalsp_dsa5;

    QList<QStringList>* fertigkeiten1_dsa5;
    QList<QStringList>* fertigkeiten2_dsa5;
    QList<QStringList>* sprachen_dsa5;
    QList<QStringList>* schriften_dsa5;

    QList<QStringList>* ktw_dsa5;
    QList<QStringList>* lep_dsa5;
    QList<QStringList>* nahkampfwaffen_dsa5;
    QList<QStringList>* fernkampfwaffen_dsa5;
    QList<QStringList>* ruestungen_dsa5;
    QList<QStringList>* schild_dsa5;
    QList<QStringList>* kSonderf_dsa5;

    QList<QStringList>* ausruestung1_dsa5;
    QList<QStringList>* ausruestung2_dsa5;
    QList<QStringList>* geld_dsa5;
    QList<QStringList>* tierAllgemein_dsa5;
    QList<QStringList>* tierAngriff_dsa5;
    QList<QStringList>* tierAktionen_dsa5;
    QList<QStringList>* tierSonderf_dsa5;

    QList<QStringList>* zauber_dsa5;
    QList<QStringList>* zaubertricks_dsa5;
    QList<QStringList>* magischeSonderfertigkeiten_dsa5;
    QList<QStringList>* leiteigenschaftMerkmal_dsa5;
    QList<QStringList>* mTradition_dsa5;

    QList<QStringList>* liturgien_dsa5;
    QList<QStringList>* segnungen_dsa5;
    QList<QStringList>* klerikaleSonderfertigkeiten_dsa5;
    QList<QStringList>* leiteigenschaftAspekt_dsa5;
    QList<QStringList>* kTradition_dsa5;

private:
    Ui::CharacterPage *ui;

    void writeTable(QList<QStringList> *list, int columns, QTableWidget* table, bool updateTableSize = false);
};

#endif // CHARACTERPAGE_H
