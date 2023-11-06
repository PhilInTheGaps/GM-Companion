#pragma once

#include "message.h"
#include "messagemodel.h"
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

    Q_PROPERTY(MessageModel *messages READ messages CONSTANT FINAL)
    [[nodiscard]] auto messages() -> MessageModel *;

    Q_INVOKABLE void markAllAsRead();
    Q_INVOKABLE void clearMessages();

    void addMessage(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body);

    AUTO_PROPERTY_VAL2(bool, hasNewErrors, false)

public slots:
    void addMessage(std::shared_ptr<Message> message);

private:
    using QObject::QObject;

    inline static MessageManager *single = nullptr;

    MessageModel m_model = MessageModel(nullptr);
};
