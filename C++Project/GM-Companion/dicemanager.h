#ifndef DICEMANAGER_H
#define DICEMANAGER_H

#include <QFrame>
#include <QList>
#include <QSpinBox>
#include <QTableWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QObject>

#include "flowlayout.h"

class DiceManager: public QObject {
    Q_OBJECT

public:
    explicit DiceManager(QObject *parent = 0);

    QFrame* generateDiceFrame();

    void setSidesList(QList<int>);

    void addDice(int);

    void resetDice();

private slots:
    void rollDice(int);

    void on_addButton_clicked();

    void on_resetButton_clicked();

private:
    QList<int> sidesList = {2, 3, 4, 6, 12, 20};

    void generateDice();
    void clearDice();

    QSignalMapper *signalMapperDice;

    QFrame* diceFrame;
    QHBoxLayout* diceFrameLayout;

    QFrame* customButtonFrame;
    QHBoxLayout* customButtonLayout;
    QSpinBox* customButtonSpinBox;

    QFrame* leftFrame;
    QVBoxLayout* leftLayout;

    QFrame* leftTopFrame;
    QHBoxLayout* leftTopLayout;

    QFrame* resultFrame;
    QVBoxLayout* resultLayout;

    QFrame* diceButtonFrame;
    FlowLayout* diceButtonLayout;

    QSpinBox* amountSpinBox;
    QSpinBox* modifierSpinBox;
    QTableWidget* diceResultTableWidget;
    QLabel* diceOutputLabel;

    QStringList headerLabels = {"dice", "roll", "result"};
};

#endif // DICEMANAGER_H
