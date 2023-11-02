#pragma once

#include "message.h"
#include "models/customsharedptrlistmodel.h"
#include <QtQml/qqmlregistration.h>

class MessageModel : public Common::CustomSharedPtrListModel<Message>
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using CustomSharedPtrListModel::CustomSharedPtrListModel;

    enum class Roles
    {
        Timestamp = Qt::UserRole,
        Type = Qt::UserRole + 1,
        Category = Qt::UserRole + 2,
        Body = Qt::UserRole + 3
    };

    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override
    {
        auto item = get(index.row());
        if (!item) return {};

        if (role == Qt::DisplayRole) return item->body;

        if (role < Qt::UserRole) return {};

        switch (static_cast<Roles>(role))
        {
        case Roles::Timestamp:
            return item->timestamp;
        case Roles::Type:
            return item->type;
        case Roles::Category:
            return item->category;
        case Roles::Body:
            return item->body;
        default:
            return {};
        }
    }

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override
    {
        auto roles = CustomSharedPtrListModel::roleNames();
        roles[static_cast<int>(Roles::Timestamp)] = "timestamp";
        roles[static_cast<int>(Roles::Type)] = "type";
        roles[static_cast<int>(Roles::Category)] = "category";
        roles[static_cast<int>(Roles::Body)] = "body";
        return roles;
    }
};
