#include "combattracker.h"
#include "ui_combattracker.h"

#include "tools/dicetool.h"

#include <QDebug>

CombatTracker::CombatTracker(QWidget *parent) : QDialog(parent), ui(new Ui::CombatTracker)
{
    ui->setupUi(this);

    combatRound = 0;
    currentIndex = 0;

    DiceTool *diceTool = new DiceTool;
    this->layout()->addWidget(diceTool);
}

CombatTracker::~CombatTracker()
{
    delete ui;
}

void CombatTracker::nextCharacter()
{
    qDebug().noquote() << "CombatTracker: Switching to next character ...";

    int nextIndex = 0;
    ui->tableWidget->item(currentIndex, 0)->setText("");

    if (combatRound == 0)
    {
        combatRound++;
        ui->spinBox_round->setValue(combatRound);
        nextIndex = getHighestInitiativeIndex();
    }
    else
    {
        if (getIsLowestInitiative(currentIndex))
        {
            combatRound++;
            ui->spinBox_round->setValue(combatRound);
            nextIndex = getHighestInitiativeIndex();
        }
        else
        {
            nextIndex = getNextHighestInitiativeIndex(currentIndex);
        }
    }

    ui->tableWidget->item(nextIndex, 0)->setText("-->");

    currentIndex = nextIndex;

    qDebug().noquote() << "CombatTracker: New index =" << currentIndex;
}

int CombatTracker::getInitiative(int index)
{
    int ini = ui->tableWidget->item(index, 2)->text().toInt();
    return ini;
}

bool CombatTracker::getIsLowestInitiative(int index)
{
    qDebug().noquote() << "CombatTracker: Checking if" << index <<"is the lowest initiative ...";

    int ini = getInitiative(index);

    bool isLowest = true;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        int temp = ui->tableWidget->item(i, 2)->text().toInt();

        if (temp < ini && temp > 0)
        {
            isLowest = false;
            break;
        }
    }

    qDebug().noquote() << isLowest;
    return isLowest;
}

int CombatTracker::getNextHighestInitiativeIndex(int index)
{
    qDebug().noquote() << "CombatTracker: Getting the next highest initiative ...";

    int ini = getInitiative(index);

    int nextIni = 0;
    int nextIniIndex = 0;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        int temp = ui->tableWidget->item(i, 2)->text().toInt();

        if (temp < ini && temp > nextIni)
        {
            nextIni = temp;
            nextIniIndex = i;
        }
    }

    qDebug().noquote() << "Initiative:" << nextIni;
    qDebug().noquote() << "Index:" << nextIniIndex;

    return nextIniIndex;
}

int CombatTracker::getHighestInitiativeIndex()
{
    qDebug().noquote() << "CombatTracker: Getting the highest initiative ...";

    int nextIni = 0;
    int nextIniIndex = 0;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        int temp = ui->tableWidget->item(i, 2)->text().toInt();

        if (temp > nextIni)
        {
            nextIni = temp;
            nextIniIndex = i;
        }
    }

    qDebug().noquote() << "Initiative:" << nextIni;
    qDebug().noquote() << "Index:" << nextIniIndex;

    return nextIniIndex;
}

void CombatTracker::resetRounds()
{
    qDebug().noquote() << "CombatTracker: Resetting rounds ...";

    ui->tableWidget->item(currentIndex, 0)->setText("");

    combatRound = 0;
    currentIndex = 0;
    ui->spinBox_round->setValue(0);
    nextCharacter();
}

void CombatTracker::clearTable()
{
    ui->tableWidget->clearContents();
}

void CombatTracker::on_pushButton_next_clicked()
{
    nextCharacter();
}

void CombatTracker::on_pushButton_clear_clicked()
{
    clearTable();
}

void CombatTracker::on_pushButton_reset_clicked()
{
    resetRounds();
}
