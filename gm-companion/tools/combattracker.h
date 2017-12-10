#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <QDialog>

namespace Ui {
class CombatTracker;
}

class CombatTracker : public QDialog
{
    Q_OBJECT

public:
    explicit CombatTracker(QWidget *parent = 0);
    ~CombatTracker();

private slots:
    void on_pushButton_next_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_reset_clicked();

private:
    Ui::CombatTracker *ui;

    int combatRound;
    int currentIndex;

    void nextCharacter();
    void clearTable();
    void resetRounds();

    int getInitiative(int index);
    int getNextHighestInitiativeIndex(int index);
    int getHighestInitiativeIndex();

    bool getIsLowestInitiative(int index);
};

#endif // COMBATTRACKER_H
