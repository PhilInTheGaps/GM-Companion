#ifndef UNSPLASHIMAGE_H
#define UNSPLASHIMAGE_H

#include <QAbstractListModel>
#include <QList>
#include <QObject>

class UnsplashImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY imageChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY imageChanged)
    Q_PROPERTY(QString authorId READ authorId WRITE setAuthorId NOTIFY imageChanged)
    Q_PROPERTY(QString authorLink READ authorLink NOTIFY imageChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY imageChanged)

public:
    explicit UnsplashImage(QObject *parent = nullptr);

    QString id() const
    {
        return m_id;
    }
    void setId(QString id)
    {
        m_id = id;
        emit imageChanged();
    }

    QString author() const
    {
        return m_author;
    }
    void setAuthor(QString author)
    {
        m_author = author;
        emit imageChanged();
    }

    QString authorId() const
    {
        return m_authorId;
    }
    void setAuthorId(QString authorId)
    {
        m_authorId = authorId;
        emit imageChanged();
    }

    QString authorLink() const
    {
        return "https://unsplash.com/@" + m_authorId;
    }

    QStringList tags() const
    {
        return m_tags;
    }
    void setTags(QStringList tags)
    {
        m_tags = tags;
        emit imageChanged();
    }
    void addTag(QString tag)
    {
        m_tags.append(tag);
        emit imageChanged();
    }

signals:
    void imageChanged();

private:
    QString m_id, m_author, m_authorId;
    QStringList m_tags;
};

class ImageListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    int rowCount(const QModelIndex &) const override
    {
        return m_items.size();
    }
    QVariant data(const QModelIndex &index, int role) const override;

    void setElements(QList<UnsplashImage *> elements);
    void clear();

public slots:
    void insert(QObject *item);
    void remove(QObject *item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QObject *> m_items = {};
};

#endif // UNSPLASHIMAGE_H
