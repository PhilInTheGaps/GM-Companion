#pragma once

#include "message.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class MessageManager : public QObject
{
    Q_OBJECT
public:
    static auto instance() -> MessageManager *;

    Q_INVOKABLE void markAllAsRead();
    Q_INVOKABLE void clearMessages();

    AUTO_PROPERTY_VAL2(bool, hasNewErrors, false)
    AUTO_PROPERTY(QList<Message *>, messages)

public slots:
    void addMessage(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body);
    void addMessage(Message *message);

private:
    using QObject::QObject;

    inline static MessageManager *single = nullptr;

    void makeMessageReadyForAdding(Message *message);
};
