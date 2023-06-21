#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QList>
#include <QVector>

class CustomObjectListModel : public QAbstractListModel
{
    Q_OBJECT
    READONLY_PROPERTY2(bool, isOwning, false)
public:
    explicit CustomObjectListModel(bool isOwning, QObject *parent = nullptr);

    [[nodiscard]] auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;
    [[nodiscard]] virtual auto data(const QModelIndex &index, int role) const -> QVariant override = 0;
    [[nodiscard]] auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
        -> QVariant override;

    void clear();
    void replaceAll(const QVector<QObject *> &objects);
    void replaceAll(const QList<QObject *> &objects);

    auto removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) -> bool override;

    auto insert(int row, QObject *object, const QModelIndex &parent = QModelIndex()) -> bool;
    void append(QObject *object, const QModelIndex &parent = QModelIndex());
    void prepend(QObject *object, const QModelIndex &parent = QModelIndex());

    [[nodiscard]] auto get(int row) const -> QObject *;
    [[nodiscard]] auto getAll() const -> QVector<QObject *>;

protected:
    void takeOwnershipIfRequired(QObject *object);
    void takeOwnershipIfRequired(const QVector<QObject *> &objects);

private:
    QVector<QObject *> m_objects;
};
