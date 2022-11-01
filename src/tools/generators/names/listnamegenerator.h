#pragma once

#include "abstractnamegenerator.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class ListNameGenerator : public AbstractNameGenerator
{
    Q_OBJECT
public:
    explicit ListNameGenerator(QObject *parent, QJsonObject json);

    Q_INVOKABLE bool generate(int count) override;

private:
    static auto getCategoryNames(const QJsonObject &json) -> QStringList;
    static auto getPrefixNames(const QJsonObject &json) -> QStringList;
    static auto getSuffixNames(const QJsonObject &json) -> QStringList;
    static auto getTypeNames(const QJsonArray &array) -> QStringList;
    static auto getRandomValue(const QList<QStringList> &names) -> QString;

    static auto jsonToStringList(const QJsonArray &json) -> QStringList;

    auto generate(int categoryIndex, int count) -> QStringList;
    auto getAvailableFirstNames(int categoryIndex) -> QList<QStringList>;
    auto getAvailableSurnames(int categoryIndex) -> QList<QStringList>;
    auto getAvailableNames(int categoryIndex, const QString &entryName) -> QList<QStringList>;
    auto getAvailablePrefixes(int prefixIndex) -> QList<QStringList>;
    auto getAvailableSuffixes(int suffixIndex) -> QList<QStringList>;
    auto getAvailableSuffixesOrPrefixes(int index, const QString &field) -> QList<QStringList>;

    QJsonObject m_json;
};
