#ifndef SIFRP_H
#define SIFRP_H

#include <QWidget>
#include <QSettings>
#include <QStringList>
#include <QList>

namespace Ui {
    class SIFRP;
}

class SIFRP : public QWidget{
    Q_OBJECT

public:
    explicit SIFRP(QWidget *parent = 0);
    ~SIFRP();

private slots:
    void on_realmComboBox_currentIndexChanged(int index);

    void on_randomRealm_clicked();

    void on_nextButton_clicked();

    void on_step2SetPlayersButton_clicked();

    void on_step2AddButton_clicked();

private:
    Ui::SIFRP *ui;

    QString getString(QString s);

    int rollDice(int amount);

    int resources [7] = {0, 0, 0, 0, 0, 0, 0};

    QStringList realms{
        "KingsLanding",
        "Dragonstone",
        "TheNorth",
        "TheIronIslands",
        "TheRiverlands",
        "TheMountainsOfTheMoon",
        "TheWesterlands",
        "TheReach",
        "TheStormlands",
        "Dorne"
    };

    QStringList resourcesList{
        "Defense",
        "Influence",
        "Lands",
        "Law",
        "Population",
        "Power",
        "Wealth"
    };

    void generateStartingResources();

    QStringList defenseDesc;
    QStringList influenceDesc;
    QStringList landsDesc;
    QStringList lawDesc;
    QStringList populationDesc;
    QStringList powerDesc;
    QStringList wealthDesc;

    int resourceModifiers[10][7]={
        {5, -5, -5, 20, 5, -5, -5},
        {20, -5, -5, 5, 0, 0, -5},
        {5, 10, 20, -10, -5, -5, -5}

    };

    int playerCount;
    int currentResourceModifyingPlayer;

    void updateResourceDescriptions();
};

#endif // SIFRP_H
