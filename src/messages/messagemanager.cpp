#include "messagemanager.h"

auto MessageManager::instance() -> MessageManager *
{
    if (!single)
    {
        single = new MessageManager(nullptr);
    }

    return single;
}

void MessageManager::addMessage(const QDateTime &timestamp, QtMsgType type, const QString &category,
                                const QString &body)
{
    auto *message = new Message(this, timestamp, type, category, body);
    addMessage(message);
}

void MessageManager::addMessage(Message *message)
{
    makeMessageReadyForAdding(message);

    m_messages.prepend(message);

    // filter errors
    if (message->type() > 0 && message->type() < 4)
    {
        hasNewErrors(true);
    }

    emit messagesChanged();
}

void MessageManager::makeMessageReadyForAdding(Message *message)
{
    if (message->thread() != this->thread())
    {
        if (message->parent() != nullptr)
        {
            message->setParent(nullptr);
        }

        message->moveToThread(this->thread());
    }

    if (message->parent() != this)
    {
        message->setParent(this);
    }
}

void MessageManager::markAllAsRead()
{
    hasNewErrors(false);
}

void MessageManager::clearMessages()
{
    while (!m_messages.isEmpty())
    {
        auto *message = m_messages.takeFirst();
        message->deleteLater();
    }

    emit messagesChanged();
    hasNewErrors(false);
}

auto MessageManager::messages() -> QQmlListProperty<Message>
{
    return {this, &m_messages};
}
