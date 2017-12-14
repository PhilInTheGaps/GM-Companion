#ifndef CONVERTERTOOL_H
#define CONVERTERTOOL_H

#include <QWidget>
#include <QSettings>
#include <QComboBox>
#include <QSpinBox>

#include "gm-companion/managers/settingsmanager.h"

namespace Ui {
class ConverterTool;
}

class ConverterTool : public QWidget
{
    Q_OBJECT

public:
    explicit ConverterTool(QWidget *parent = 0);
    ~ConverterTool();

    struct Unit{
        QString name;
        double refUnits;
    };

private slots:
    // Add Units
    void on_comboBox_unitType_currentIndexChanged(int index);
    void on_pushButton_addUnit_clicked();

    // Length
    void on_comboBox_length1_currentIndexChanged(int index);
    void on_comboBox_length2_currentIndexChanged(int index);
    void on_doubleSpinBox_length1_valueChanged(double arg1);

    // Area
    void on_comboBox_area1_currentIndexChanged(int index);
    void on_comboBox_area2_currentIndexChanged(int index);
    void on_doubleSpinBox_area1_valueChanged(double arg1);

    // Volume
    void on_comboBox_volume1_currentIndexChanged(int index);
    void on_comboBox_volume2_currentIndexChanged(int index);
    void on_doubleSpinBox_volume1_valueChanged(double arg1);

    // Weight
    void on_comboBox_weight1_currentIndexChanged(int index);
    void on_comboBox_weight2_currentIndexChanged(int index);
    void on_doubleSpinBox_weight1_valueChanged(double arg1);

    // Money
    void on_comboBox_money1_currentIndexChanged(int index);
    void on_comboBox_money2_currentIndexChanged(int index);
    void on_doubleSpinBox_money1_valueChanged(double arg1);

private:
    Ui::ConverterTool *ui;

    SettingsManager *settingsManager;

    void getAllUnits();

    void convertUnits(int index1, int index2, QList<Unit> list, double units, QDoubleSpinBox *box);

    QList<Unit> getUnits(QString arrayName, int type, QComboBox *box1, QComboBox *box2, QString addon = "");
    void addUnit(QString arrayName, Unit unit, QList<Unit> list, QComboBox *box1, QComboBox *box2);

    QList<Unit> lengthUnits;
    QList<Unit> areaUnits;
    QList<Unit> volumeUnits;
    QList<Unit> weightUnits;
    QList<Unit> moneyUnits;
};

#endif // CONVERTERTOOL_H
