#ifndef CHAREDITOR_H
#define CHAREDITOR_H

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>
#include <QSettings>
#include "managers/settingsmanager.h"

namespace Ui {
class CharEditor;
}

class CharEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit CharEditor(QWidget *parent = 0);
    ~CharEditor();

    void load(int index);

private slots:
    void on_systemComboBox_currentIndexChanged(int index);

    void on_saveButton_clicked();

    void on_cancelButton_clicked();

    void on_addRowSkillsTable1_generic_clicked();

    void on_removeRowSkillsTable1_generic_clicked();

    void on_addRowSkillsTable2_generic_clicked();

    void on_removeRowSkillsTable2_generic_clicked();

    void on_addRowSkillsTable3_generic_clicked();

    void on_removeRowSkillsTable3_generic_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_iconButton_clicked();

    void on_addVorteil_dsa5_clicked();

    void on_removeVorteil_dsa5_clicked();

    void on_addNachteil_dsa5_clicked();

    void on_removeNachteil_dsa5_clicked();

    void on_addSonderf_dsa5_clicked();

    void on_removeSonderf_dsa5_clicked();

    void on_addSprache_dsa5_clicked();

    void on_removeSprache_dsa5_clicked();

    void on_addSchrift_dsa5_clicked();

    void on_removeSchrift_dsa5_clicked();

    void on_addKSonderf_dsa5_clicked();

    void on_removeKSonderf_dsa5_clicked();

    void on_addNahkampf_dsa5_clicked();

    void on_removeNahkampf_dsa5_clicked();

    void on_addRuestung_dsa5_clicked();

    void on_removeRuestung_dsa5_clicked();

    void on_addFernkampf_dsa5_clicked();

    void on_removeFernkampf_dsa5_clicked();

    void on_addSchild_dsa5_clicked();

    void on_removeSchild_dsa5_clicked();

    void on_addAusruestung1_dsa5_clicked();

    void on_removeAusruestung1_dsa55_clicked();

    void on_addAusruestung2_dsa5_clicked();

    void on_removeAusruestung2_dsa5_clicked();

    void on_addTierSonderf_dsa5_clicked();

    void on_removeTierSonderf_dsa5_clicked();

    void on_addTierAngriff_dsa5_clicked();

    void on_removeTierAngriff_dsa5_clicked();

    void on_addTierAktionen_dsa5_clicked();

    void on_removeTierAktionen_dsa5_clicked();

    void on_addZauber_dsa5_clicked();

    void on_removeZauber_dsa5_clicked();

    void on_addZaubertrick_dsa5_clicked();

    void on_removeZaubertrick_dsa5_clicked();

    void on_addMSonderf_dsa5_clicked();

    void on_removeMSonderf_dsa5_clicked();

    void on_addLitugie_dsa5_clicked();

    void on_removeLiturgie_dsa5_clicked();

    void on_addSegnung_dsa5_clicked();

    void on_removeSegnung_dsa5_clicked();

    void on_addKleriSonderf_dsa5_clicked();

    void on_removeKleriSonderf_dsa5_clicked();

    void on_addLineQualities_sifrp_clicked();

    void on_removeLineQualities_sifrp_clicked();

    void on_addRowEquipment_sifrp_clicked();

    void on_removeRowEquipment_sifrp_clicked();

private:
    Ui::CharEditor *ui;

    SettingsManager* settingsManager;

    void save();

    void addRow(QTableWidget* table);
    void removeRow(QTableWidget* table);

    QString loadCharacterFile;

    void writeTable(QString indicator, int columns, QTableWidget* table, bool updateTableSize = false);

    void writeToFile(QTableWidget* table, QString indicator, int columns);

    QString displayName;
};

#endif // CHAREDITOR_H
