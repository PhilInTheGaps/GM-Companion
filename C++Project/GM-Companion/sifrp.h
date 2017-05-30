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

    void on_foundingComboBox_currentIndexChanged(int index);

    void on_foundingRandomButton_clicked();

    void on_eventComboBox_currentIndexChanged(int index);

    void on_eventRandomButton_clicked();

    void on_foundingApplyButton_clicked();

    void on_eventApplyButton_clicked();

    void on_step4InvestInDefenseButton_clicked();

private:
    Ui::SIFRP *ui;

    QString getString(QString s);

    int rollDice(int amount);

    int resources[7] = {0, 0, 0, 0, 0, 0, 0};

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

    QList<QList<int>> resourceModifiers;

    int playerCount;
    int currentResourceModifyingPlayer;

    void updateResourceDescriptions();

    void updateFoundingExample(int index);

    QStringList foundingExamples = {
        "Age of Heroes",
        "Andal Invasion",
        "Rhoynar Invasion",
        "Aegon’s Conquest",
        "Blackfyre Rebellion",
        "War of the Usurper"
    };

    void updateEventDescription(int index);

    void calculateEventCount();

    void calculateMinEventCount();

    int remainingMinEventCount;

    void calculateEventMods();

    enum Holding{
        Defense,
        Influence,
        Land,
        Law,
        Population,
        Wealth
    };

    void addHolding(Holding holding, QString secondColumn, QString thirdColumn);
};

#endif // SIFRP_H
