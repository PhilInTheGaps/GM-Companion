#include "messagemanager.h"

MessageManager::MessageManager(QObject *parent)
    : QObject{parent}, a_hasNewErrors(false)
{

}

auto MessageManager::instance() -> MessageManager*
{
    if (!single)
    {
        single = new MessageManager();
    }

    return single;
}

void MessageManager::addMessage(const QDateTime &timestamp, QtMsgType type, const QString &body)
{
    auto *message = new Message(this, timestamp, type, body);
    addMessage(message);
}

void MessageManager::addMessage(Message *message)
{
    a_messages.prepend(message);

    // filter errors
    if (message->type() > 0 && message->type() < 4 )
    {
        hasNewErrors(true);
    }

    emit messagesChanged(messages());
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
