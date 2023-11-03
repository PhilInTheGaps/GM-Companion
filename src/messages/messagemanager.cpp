#include "messagemanager.h"
#include <QMutexLocker>

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
    addMessage(std::make_shared<Message>(timestamp, type, category, body));
}

void MessageManager::addMessage(std::shared_ptr<Message> message)
{
    if (QThread::currentThread() != this->thread())
    {
        QTimer::singleShot(0, this, [this, message = std::move(message)]() { addMessage(message); });
        return;
    }

    QMutexLocker const lock(&m_mutex); // lock for messages from different threads

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
