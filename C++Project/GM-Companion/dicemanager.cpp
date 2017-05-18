#include "dicemanager.h"

#include <QPushButton>

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
        i1->setText(QString::number(i+1)+"D"+QString::number(sides));
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
    signalMapperDice = new QSignalMapper;

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
    amountSpinBox->setMinimumSize(50, 150);
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
    modifierSpinBox->setMinimumSize(100, 150);
    modifierSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    leftTopLayout->addWidget(modifierSpinBox);

    resultFrame = new QFrame;
    resultLayout = new QVBoxLayout;
    resultFrame->setLayout(resultLayout);
    resultFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    resultFrame->setFrameShape(QFrame::Box);
    leftLayout->addWidget(resultFrame);

    QLabel* resultLabel = new QLabel;
    resultLabel->setText("Result:");
    QFont f( "MS Shell Dlg 2", 50, QFont::Bold);
    resultLabel->setFont(f);
    resultLayout->addWidget(resultLabel);

    diceOutputLabel = new QLabel;
    diceOutputLabel->setAlignment(Qt::AlignCenter);
    diceOutputLabel->setMinimumSize(300, 300);
    QFont fr( "MS Shell Dlg 2", 80);
    diceOutputLabel->setFont(fr);
    diceOutputLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    resultLayout->addWidget(diceOutputLabel);

    diceResultTableWidget = new QTableWidget;
    diceResultTableWidget->setColumnCount(3);
    diceResultTableWidget->setHorizontalHeaderLabels(headerLabels);
    diceResultTableWidget->setMinimumWidth(350);
    diceResultTableWidget->setMaximumWidth(350);
    diceFrameLayout->addWidget(diceResultTableWidget);

    for (int sides : sidesList){
        QPushButton *button = new QPushButton;
        button->setText("D"+QString::number(sides));
        //button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //button->setMinimumSize(200, 200);
        button->setStyleSheet("min-width: 50; min-height: 50; max-width: 200; max-height: 200;");
        //button->setMaximumSize(100, 100);

        diceButtonLayout->addWidget(button);

        connect(button, SIGNAL(clicked()), signalMapperDice, SLOT(map()));
        signalMapperDice->setMapping(button, sides);
    }

    connect(signalMapperDice, SIGNAL(mapped(int)), this, SLOT(rollDice(int)));

    return diceFrame;
}
