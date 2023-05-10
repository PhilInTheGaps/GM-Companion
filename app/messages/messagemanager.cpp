#include "messagemanager.h"

auto MessageManager::instance() -> MessageManager *
{
    if (!single)
    {
        single = new MessageManager();
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

    a_messages.prepend(message);

    // filter errors
    if (message->type() > 0 && message->type() < 4)
    {
        hasNewErrors(true);
    }

    emit messagesChanged(messages());
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
    auto list = messages();

    while (!list.isEmpty())
    {
        auto *message = list.takeFirst();
        message->deleteLater();
    }

    messages({});
    hasNewErrors(false);
}
