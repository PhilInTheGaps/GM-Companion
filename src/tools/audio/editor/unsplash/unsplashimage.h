#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class UnsplashImage : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString authorLink READ authorLink NOTIFY authorIdChanged)

    AUTO_PROPERTY(QString, id)
    AUTO_PROPERTY(QString, author)
    AUTO_PROPERTY(QString, authorId)
    AUTO_PROPERTY(QStringList, tags)

public:
    explicit UnsplashImage(QObject *parent = nullptr);

    [[nodiscard]] auto authorLink() const -> QString
    {
        return QStringLiteral("https://unsplash.com/@%1").arg(authorId());
    }

    void addTag(const QString &tag)
    {
        a_tags.append(tag);
        emit tagsChanged(tags());
    }
};

class ImageListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QAbstractListModel::QAbstractListModel;

    [[nodiscard]] auto rowCount(const QModelIndex &) const -> int override
    {
        return m_items.size();
    }
    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override;

    void setElements(QList<UnsplashImage *> elements);
    void clear();

public slots:
    void insert(QObject *item);
    void remove(QObject *item);

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

private:
    QList<QObject *> m_items = {};
};

Q_DECLARE_METATYPE(ImageListModel *)
