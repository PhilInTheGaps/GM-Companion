#include "unsplashimage.h"

UnsplashImage::UnsplashImage(QObject *parent) : QObject(parent)
{
}

auto ImageListModel::data(const QModelIndex &index, int /*role*/) const -> QVariant
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void ImageListModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

void ImageListModel::remove(QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items.at(i) == item)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.remove(i);
            endRemoveRows();
            break;
        }
    }
}

auto ImageListModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void ImageListModel::clear()
{
    while (!m_items.empty())
    {
        remove(m_items.constFirst());
    }
}

void ImageListModel::setElements(QList<UnsplashImage *> elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
