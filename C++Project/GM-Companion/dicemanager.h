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

private slots:
    void rollDice(int);

private:
    QList<int> sidesList = {2, 3, 4, 6, 12, 20};

    QSignalMapper *signalMapperDice;

    QFrame* diceFrame;
    QHBoxLayout* diceFrameLayout;

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
