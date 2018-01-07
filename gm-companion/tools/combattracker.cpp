#include "combattracker.h"
#include "ui_combattracker.h"

#include "gm-companion/tools/dicetool.h"

#include <QDebug>
#include <QTableWidgetItem>

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

    // Create a new tablewidget item in case it does not already exist due to "Clear Table"
    if (ui->tableWidget->item(currentIndex, 0) == NULL)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText("");
        ui->tableWidget->setItem(currentIndex, 0, item);
    }
    else
        ui->tableWidget->item(currentIndex, 0)->setText("");

    // If it is the first round, find character with highest INI
    if (combatRound == 0)
    {
        combatRound++;
        ui->spinBox_round->setValue(combatRound);
        nextIndex = getHighestInitiativeIndex();
    }
    else
    {
        // Check if current character has the lowest INI, if not, switch to next one
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

    // Create a new tablewidget item in case it does not already exist due to "Clear Table"
    if (ui->tableWidget->item(nextIndex, 0) == NULL)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText("");
        ui->tableWidget->setItem(nextIndex, 0, item);
    }

    ui->tableWidget->item(nextIndex, 0)->setText("-->");
    ui->tableWidget->selectRow(nextIndex);

    currentIndex = nextIndex;

    qDebug().noquote() << "CombatTracker: New index =" << currentIndex;
}

int CombatTracker::getInitiative(int index)
{
    if (ui->tableWidget->item(index, 2) != NULL)
    {
        int ini = ui->tableWidget->item(index, 2)->text().toInt();
        return ini;
    }
    else
        return 0;
}

bool CombatTracker::getIsLowestInitiative(int index)
{
    qDebug().noquote() << "CombatTracker: Checking if" << index <<"is the lowest initiative ...";

    int ini = getInitiative(index);

    bool isLowest = true;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if (ui->tableWidget->item(i, 2) != NULL)
        {
            int temp = ui->tableWidget->item(i, 2)->text().toInt();

            if ((temp < ini && temp > 0) || (i > index && ini == temp))
            {
                isLowest = false;
                break;
            }
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

    bool foundSameIni = false;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if (ui->tableWidget->item(i, 2) != NULL)
        {
            int temp = ui->tableWidget->item(i, 2)->text().toInt();

            if ((temp < ini && temp > nextIni) || (i > index && temp == ini && !foundSameIni))
            {
                nextIni = temp;
                nextIniIndex = i;

                if (i > index && temp == ini && !foundSameIni)
                    foundSameIni = true;
            }
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
        if (ui->tableWidget->item(i, 2) != NULL)
        {
            int temp = ui->tableWidget->item(i, 2)->text().toInt();

            if (temp > nextIni)
            {
                nextIni = temp;
                nextIniIndex = i;
            }
        }
    }

    qDebug().noquote() << "Initiative:" << nextIni;
    qDebug().noquote() << "Index:" << nextIniIndex;

    return nextIniIndex;
}

void CombatTracker::resetRounds()
{
    qDebug().noquote() << "CombatTracker: Resetting rounds ...";

    if (ui->tableWidget->item(currentIndex, 0) != NULL)
    {
        ui->tableWidget->item(currentIndex, 0)->setText("");
    }

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
