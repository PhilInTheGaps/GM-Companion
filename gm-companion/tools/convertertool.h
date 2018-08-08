#ifndef CONVERTERTOOL_H
#define CONVERTERTOOL_H

#include "gm-companion/settings/settingsmanager.h"

#include <QObject>
#include <QStringList>
#include <QList>

class ConverterTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList units READ units NOTIFY unitsChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QString unit1 READ unit1 NOTIFY unit1Changed)
    Q_PROPERTY(QString unit2 READ unit2 NOTIFY unit2Changed)
    Q_PROPERTY(bool isUnit1Next READ isUnit1Next NOTIFY isUnit1NextChanged)
    Q_PROPERTY(QString unit2ValueString READ unit2ValueString NOTIFY unit2ValueStringChanged)

public:
    explicit ConverterTool(QObject *parent = nullptr);

    QStringList units() { return m_units[m_typeIndex][m_categoryIndex];}
    QStringList categories() const { return m_categories[m_typeIndex];}
    QString unit1() const { return m_units[m_typeIndex][m_categoryIndex][m_unitIndex1]; }
    QString unit2() const { return m_units[m_typeIndex][m_categoryIndex][m_unitIndex2]; }
    bool isUnit1Next() const { return m_isUnit1Next; }
    QString unit2ValueString() const { return m_unit2ValueString; }

    Q_INVOKABLE void convert();

    Q_INVOKABLE void updateUnits();
    Q_INVOKABLE void addUnit(QString name, QString refUnits, QString type);
    Q_INVOKABLE QString refUnitName(QString unit);

    Q_INVOKABLE void deleteUnit(int index);

    Q_INVOKABLE void setCurrentType(int index);
    Q_INVOKABLE void setCurrentCategory(int index);
    Q_INVOKABLE void setUnit(int index);
    Q_INVOKABLE void setIsUnit1Next(bool isNext);
    Q_INVOKABLE void setUnit1ValueString(QString value);

signals:
    void unitsChanged();
    void categoriesChanged();
    void unit1Changed();
    void unit2Changed();
    void isUnit1NextChanged();
    void unit2ValueStringChanged();

private:
    QStringList m_unitPaths;
    QStringList m_unitTypes = {"LengthUnits", "AreaUnits", "VolumeUnits", "WeightUnits", "MoneyUnits"};

    SettingsManager sManager;
    void addAddonUnits();

    double textToNumber(QString text);

    QList<QStringList> m_categories;
    QList<QList<QStringList>> m_units;
    QList<QList<QList<double>>> m_unitValues;

    double unitValue(int index, int category);

    int m_typeIndex = 0;
    int m_categoryIndex = 0;
    int m_categoryIndex1 = 0;
    int m_categoryIndex2 = 0;
    int m_unitIndex1 = 0;
    int m_unitIndex2 = 0;
    bool m_isUnit1Next = true;
    QString m_unit2ValueString;
    QString m_unit1ValueString;
};

#endif // CONVERTERTOOL_H
