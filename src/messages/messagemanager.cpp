#include "messagemanager.h"

auto MessageManager::instance() -> MessageManager *
{
    if (!single)
    {
        single = new MessageManager(nullptr);
    }

    return single;
}

/// Thread safe
void MessageManager::addMessage(const QDateTime &timestamp, QtMsgType type, const QString &category,
                                const QString &body)
{
    auto message = std::make_shared<Message>(timestamp, type, category, body);

    QMetaObject::invokeMethod(
        instance(), [message = std::move(message)]() { instance()->addMessage(std::move(message)); },
        Qt::ConnectionType::AutoConnection);
}

/// Not thread safe
void MessageManager::addMessage(std::shared_ptr<Message> message)
{
    // filter errors
    if (message->type > QtMsgType::QtDebugMsg && message->type < QtMsgType::QtInfoMsg)
    {
        hasNewErrors(true);
    }

    m_model.prepend(std::move(message));
}

void MessageManager::markAllAsRead()
{
    hasNewErrors(false);
}

void MessageManager::clearMessages()
{
    m_model.clear();
    hasNewErrors(false);
}

auto MessageManager::messages() -> MessageModel *
{
    return &m_model;
}
