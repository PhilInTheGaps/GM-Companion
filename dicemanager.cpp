#include "dicemanager.h"

#include <QPushButton>
#include <QHeaderView>
#include <QDebug>

DiceManager::DiceManager(QObject *parent) : QObject(parent)
{

}

void DiceManager::rollDice(int sides){

    int amount = amountSpinBox->value();
    int modifier = modifierSpinBox->value();
    int result = 0;

    diceResultTableWidget->insertRow(0);

    for (int i = 0; i<amount; i++){
        QTableWidgetItem *i1 = new QTableWidgetItem;
        QTableWidgetItem *i2 = new QTableWidgetItem;
        QTableWidgetItem *i3 = new QTableWidgetItem;

        int temp = rand() % sides+1;
        i1->setText(QString::number(i+1)+tr("D")+QString::number(sides));
        i2->setText(QString::number(temp));
        result += temp;
        i3->setText(QString::number(result));

        diceResultTableWidget->insertRow(0);

        diceResultTableWidget->setItem(0, 0, i1);
        diceResultTableWidget->setItem(0, 1, i2);
        diceResultTableWidget->setItem(0, 2, i3);
    }

    result += modifier;

    diceOutputLabel->setText(QString::number(result));
}

QFrame* DiceManager::generateDiceFrame(){
    diceFrame = new QFrame;
    diceFrameLayout = new QHBoxLayout;
    diceFrame->setLayout(diceFrameLayout);

    leftFrame = new QFrame;
    leftLayout = new QVBoxLayout;
    leftFrame->setLayout(leftLayout);
    diceFrameLayout->addWidget(leftFrame);

    leftTopFrame = new QFrame;
    leftTopLayout = new QHBoxLayout;
    leftTopFrame->setLayout(leftTopLayout);
    leftTopFrame->setFrameShape(QFrame::Box);
    leftLayout->addWidget(leftTopFrame);

    QFont fs( "MS Shell Dlg 2", 40);

    amountSpinBox = new QSpinBox;
    amountSpinBox->setMaximum(999);
    amountSpinBox->setMinimum(1);
    amountSpinBox->setValue(1);
    amountSpinBox->setFont(fs);
    amountSpinBox->setMinimumSize(50, 0); //50, 150
    amountSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    leftTopLayout->addWidget(amountSpinBox);

    QLabel* xLabel = new QLabel;
    xLabel->setText(" x ");
    xLabel->setFont(fs);
    xLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    leftTopLayout->addWidget(xLabel);

    diceButtonFrame = new QFrame;
    diceButtonLayout = new FlowLayout;
    diceButtonLayout->setAlignment(Qt::AlignCenter);
    diceButtonFrame->setLayout(diceButtonLayout);
    diceButtonFrame->setMinimumWidth(150);
    diceButtonFrame->setMaximumWidth(400);
    diceButtonFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    diceButtonFrame->setFrameShape(QFrame::Box);
    leftTopLayout->addWidget(diceButtonFrame);

    QLabel* plusLabel = new QLabel;
    plusLabel->setText(" + ");
    plusLabel->setFont(fs);
    plusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    leftTopLayout->addWidget(plusLabel);

    modifierSpinBox = new QSpinBox;
    modifierSpinBox->setMaximum(999);
    modifierSpinBox->setMinimum(-999);
    modifierSpinBox->setValue(0);
    modifierSpinBox->setFont(fs);
    modifierSpinBox->setMinimumSize(100, 0); //100, 150
    modifierSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    leftTopLayout->addWidget(modifierSpinBox);

    resultFrame = new QFrame;
    resultLayout = new QVBoxLayout;
    resultFrame->setLayout(resultLayout);
    resultFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    resultFrame->setFrameShape(QFrame::Box);
    leftLayout->addWidget(resultFrame);

    // Custom Buttons
    customButtonFrame = new QFrame;
    customButtonLayout = new QHBoxLayout;
    customButtonLayout->setAlignment(Qt::AlignLeft);
    customButtonFrame->setLayout(customButtonLayout);
    customButtonFrame->setMinimumWidth(150);
    customButtonFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    customButtonFrame->setFrameShape(QFrame::Box);
    leftLayout->addWidget(customButtonFrame);

    QLabel* cLabel = new QLabel(tr("Custom Dice: "));
    customButtonLayout->addWidget(cLabel);
    cLabel->setFont(fs);

    customButtonSpinBox = new QSpinBox;
    customButtonSpinBox->setMaximum(999);
    customButtonSpinBox->setMinimum(1);
    customButtonSpinBox->setValue(1);
    customButtonSpinBox->setFont(fs);
    customButtonLayout->addWidget(customButtonSpinBox);

    QPushButton* addButton = new QPushButton;
    addButton->setText(tr("Add Die"));
    addButton->setStyleSheet("min-width: 50; min-height: 50; max-height: 200;");
    customButtonLayout->addWidget(addButton);
    connect(addButton, SIGNAL(clicked()), SLOT(on_addButton_clicked()));

    QPushButton* resetButton = new QPushButton;
    resetButton->setText(tr("Reset Dice"));
    resetButton->setStyleSheet("min-width: 50; min-height: 50; max-height: 200;");
    customButtonLayout->addWidget(resetButton);
    connect(resetButton, SIGNAL(clicked()), SLOT(on_resetButton_clicked()));

    // Result
    QLabel* resultLabel = new QLabel;
    resultLabel->setText(tr("Result:"));
    QFont f( "MS Shell Dlg 2", 50, QFont::Bold);
    resultLabel->setFont(f);
    resultLayout->addWidget(resultLabel);

    diceOutputLabel = new QLabel;
    diceOutputLabel->setAlignment(Qt::AlignCenter);
    diceOutputLabel->setMinimumSize(300, 0); //300, 300
    QFont fr( "MS Shell Dlg 2", 80);
    diceOutputLabel->setFont(fr);
    diceOutputLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    resultLayout->addWidget(diceOutputLabel);

    diceResultTableWidget = new QTableWidget;
    diceResultTableWidget->setColumnCount(3);
    diceResultTableWidget->setHorizontalHeaderLabels(headerLabels);
    diceResultTableWidget->horizontalHeader()->stretchLastSection();
    diceResultTableWidget->setMinimumWidth(350);
    diceResultTableWidget->setMaximumWidth(350);
    diceFrameLayout->addWidget(diceResultTableWidget);

    generateDice();

    return diceFrame;
}

void DiceManager::generateDice(){
    signalMapperDice = new QSignalMapper;

    for (int sides : sidesList){
        QPushButton *button = new QPushButton;
        button->setText(tr("D")+QString::number(sides));
        button->setStyleSheet("min-width: 50; min-height: 50; max-width: 200; max-height: 200;");

        diceButtonLayout->addWidget(button);

        connect(button, SIGNAL(clicked()), signalMapperDice, SLOT(map()), Qt::UniqueConnection);
        signalMapperDice->setMapping(button, sides);
    }

    connect(signalMapperDice, SIGNAL(mapped(int)), this, SLOT(rollDice(int)));
}

void DiceManager::clearDice(){
    signalMapperDice->deleteLater();

    while (auto item = diceButtonLayout->takeAt(0)) {
        delete item->widget();
    }

}

void DiceManager::on_addButton_clicked(){
    clearDice();
    addDice(customButtonSpinBox->value());
    generateDice();
}

void DiceManager::addDice(int sides){
    sidesList.push_back(sides);
}

void DiceManager::on_resetButton_clicked(){
    clearDice();
    resetDice();
    generateDice();
}

void DiceManager::resetDice(){
    sidesList = {2, 3, 4, 6, 12, 20};
}
