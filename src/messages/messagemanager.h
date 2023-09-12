#pragma once

#include "message.h"
#include "qmlsingletonfactory.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class MessageManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_SINGLETON_FACTORY(MessageManager)

public:
    MessageManager() = delete;
    static auto instance() -> MessageManager *;

    Q_INVOKABLE void markAllAsRead();
    Q_INVOKABLE void clearMessages();

    AUTO_PROPERTY_VAL2(bool, hasNewErrors, false)
    Q_PROPERTY(QQmlListProperty<Message> messages READ messages NOTIFY messagesChanged FINAL)
    auto messages() -> QQmlListProperty<Message>;

public slots:
    void addMessage(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body);
    void addMessage(Message *message);

signals:
    void messagesChanged();

private:
    using QObject::QObject;

    inline static MessageManager *single = nullptr;
    QList<Message *> m_messages;

    void makeMessageReadyForAdding(Message *message);
};
