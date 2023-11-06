#pragma once

#include "audiofile.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

class AudioFileModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
    AUTO_PROPERTY(QString, name)

public:
    using QAbstractListModel::QAbstractListModel;

    [[nodiscard]] auto rowCount(const QModelIndex & /*parent*/) const -> int override
    {
        return static_cast<int>(m_items.size());
    }
    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override;

    [[nodiscard]] auto elements() const -> QList<QObject *>
    {
        return m_items;
    }
    void setElements(const QList<AudioFile *> &elements);

    void clear();
    [[nodiscard]] auto isEmpty() const -> bool
    {
        return m_items.isEmpty();
    }

    void insert(int index, QObject *item);
    void remove(const QObject *item);
    void remove(int index);
    void append(QObject *item);
    auto moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent,
                 int destinationChild) -> bool;

signals:
    void isEmptyChanged();

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

private:
    QList<QObject *> m_items = {};
};

Q_DECLARE_METATYPE(AudioFileModel *)
