#ifndef AUDIOELEMENTMODEL_H
#define AUDIOELEMENTMODEL_H

#include <QAbstractListModel>
#include "audioelement.h"

// Model for QML
class AudioElementModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit AudioElementModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;
    void setElements(QList<AudioElement*> elements);
    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    void setSearchTerm(const QString& term);

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

signals:
    void isEmptyChanged();
    void nameChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
    QString m_name;
};

/**
 * @brief A model consisting of element models
 */
class AudioElementModelModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)

public:
    AudioElementModelModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;
    void setElements(QList<AudioElementModel *> elements);
    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }
    AudioElementModel* element(int index) { if (m_items.size() > 0) return static_cast<AudioElementModel*>(m_items[index]); else return nullptr; }

    void setSearchTerm(const QString& term);

public slots:
    void insert(QObject* item);
    void append(QObject* item);
    void remove(QObject* item);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

#endif // AUDIOELEMENTMODEL_H
