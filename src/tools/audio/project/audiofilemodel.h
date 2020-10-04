#ifndef AUDIOFILEMODEL_H
#define AUDIOFILEMODEL_H

#include "audiofile.h"
#include <QAbstractListModel>

class AudioFileModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    AudioFileModel(QObject *parent) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    QVector<QObject*> elements() const { return m_items; }
    void setElements(const QList<AudioFile*>& elements);

    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    void insert(int index, QObject* item);
    void remove(QObject* item);
    void append(QObject* item);
    bool moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild);

signals:
    void isEmptyChanged();
    void nameChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
    QString m_name;
};

#endif // AUDIOFILEMODEL_H
