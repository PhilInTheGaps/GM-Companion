#ifndef DICETOOL_H
#define DICETOOL_H

#include <QWidget>
#include <QList>
#include <QSignalMapper>

namespace Ui {
class DiceTool;
}

class DiceTool : public QWidget
{
    Q_OBJECT

public:
    explicit DiceTool(QWidget *parent = 0);
    ~DiceTool();

private slots:
    void on_pushButton_addDie_clicked();

    void on_pushButton_resetDice_clicked();

    void on_pushButton_roll_clicked();

private:
    Ui::DiceTool *ui;

    QSignalMapper *signalMapperDice;

    QList<int> sidesList = {2, 3, 4, 6, 12, 20};    // Default Dice

    void rollDice(int sides);

    void generateDice();
};

#endif // DICETOOL_H
