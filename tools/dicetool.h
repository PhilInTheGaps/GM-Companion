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
    void rollDice(int sides);

    void on_pushButton_addDie_clicked();

    void on_pushButton_resetDice_clicked();

private:
    Ui::DiceTool *ui;

    QSignalMapper *signalMapperDice;

    QList<int> sidesList = {2, 3, 4, 6, 12, 20};    // Default Dice

    void generateDice();
};

#endif // DICETOOL_H
