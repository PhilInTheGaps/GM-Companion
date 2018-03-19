#ifndef CONVERTERTOOL_H
#define CONVERTERTOOL_H

#include <QObject>
#include <QStringList>
#include <QList>

class ConverterTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList lengthUnits READ lengthUnits NOTIFY lengthUnitsChanged)
    Q_PROPERTY(QStringList areaUnits READ areaUnits NOTIFY areaUnitsChanged)
    Q_PROPERTY(QStringList volumeUnits READ volumeUnits NOTIFY volumeUnitsChanged)
    Q_PROPERTY(QStringList weightUnits READ weightUnits NOTIFY weightUnitsChanged)
    Q_PROPERTY(QStringList moneyUnits READ moneyUnits NOTIFY moneyUnitsChanged)

public:
    explicit ConverterTool(QObject *parent = nullptr);

    QStringList lengthUnits();
    QStringList areaUnits();
    QStringList volumeUnits();
    QStringList weightUnits();
    QStringList moneyUnits();

    Q_INVOKABLE QString convertLength(QString unit1, QString unit2, QString value);
    Q_INVOKABLE QString convertArea(QString unit1, QString unit2, QString value);
    Q_INVOKABLE QString convertVolume(QString unit1, QString unit2, QString value);
    Q_INVOKABLE QString convertWeight(QString unit1, QString unit2, QString value);
    Q_INVOKABLE QString convertMoney(QString unit1, QString unit2, QString value);

    Q_INVOKABLE void updateUnits();
    Q_INVOKABLE void addUnit(QString name, QString refUnits, QString type);
    Q_INVOKABLE QString refUnitName(QString unit);

signals:
    void lengthUnitsChanged();
    void areaUnitsChanged();
    void volumeUnitsChanged();
    void weightUnitsChanged();
    void moneyUnitsChanged();

private:
    QStringList unitPaths;
    QStringList unitTypes = {"LengthUnits", "AreaUnits", "VolumeUnits", "WeightUnits", "MoneyUnits"};

    double textToNumber(QString text);

    QStringList l_lengthUnits;
    double lengthUnitValue(QString unit);
    QList<double> l_lengthUnitsValues;

    QStringList l_areaUnits;
    double areaUnitValue(QString unit);
    QList<double> l_areaUnitsValues;

    QStringList l_volumeUnits;
    double volumeUnitValue(QString unit);
    QList<double> l_volumeUnitsValues;

    QStringList l_weightUnits;
    double weightUnitValue(QString unit);
    QList<double> l_weightUnitsValues;

    QStringList l_moneyUnits;
    double moneyUnitValue(QString unit);
    QList<double> l_moneyUnitsValues;
};

#endif // CONVERTERTOOL_H
