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

    int rowCount(const QModelIndex &) const override
    {
        return m_items.size();
    }
    QVariant data(const QModelIndex &index, int role) const override;

    QList<QObject *> elements() const
    {
        return m_items;
    }
    void setElements(const QList<AudioFile *> &elements);

    void clear();
    auto isEmpty() const -> bool
    {
        return m_items.isEmpty();
    }

    void insert(int index, QObject *item);
    void remove(QObject *item);
    void remove(int index);
    void append(QObject *item);
    bool moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent,
                 int destinationChild);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QObject *> m_items = {};
};

Q_DECLARE_METATYPE(AudioFileModel *)
