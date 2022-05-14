#include "messagedispatcher.h"
#include "messagemanager.h"

MessageDispatcher::MessageDispatcher()
{

}

void MessageDispatcher::dispatch(Message *message)
{
    emit dispatchMessage(message);
}

void MessageDispatcher::dispatch(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body)
{
    MessageDispatcher dispatcher;
    connect(&dispatcher, &MessageDispatcher::dispatchMessage, MessageManager::instance(), qOverload<Message*>(&MessageManager::addMessage));

    auto *message = new Message(nullptr, timestamp, type, category, body);
    message->moveToThread(MessageManager::instance()->thread());

    dispatcher.dispatch(message);
}
