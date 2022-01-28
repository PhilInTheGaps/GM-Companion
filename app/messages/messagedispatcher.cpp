#include "messagedispatcher.h"
#include "messagemanager.h"

MessageDispatcher::MessageDispatcher(const QDateTime &timestamp, QtMsgType type, const QString &body)
{
    connect(this, &MessageDispatcher::dispatchMessage,MessageManager::instance(), qOverload<Message*>(&MessageManager::addMessage));

    auto *message = new Message(nullptr, timestamp, type, body);
    message->moveToThread(MessageManager::instance()->thread());

    emit dispatchMessage(message);

    this->deleteLater();
}
