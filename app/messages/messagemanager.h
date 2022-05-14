#pragma once

#include <QObject>
#include "message.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class MessageManager : public QObject
{
    Q_OBJECT
public:
    static auto instance() -> MessageManager*;

    Q_INVOKABLE void markAllAsRead();
    Q_INVOKABLE void clearMessages();

    AUTO_PROPERTY(bool, hasNewErrors)
    AUTO_PROPERTY(QList<Message*>, messages)

public slots:
    void addMessage(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body);
    void addMessage(Message *message);

private:
    explicit MessageManager(QObject *parent = nullptr);

    inline static MessageManager *single = nullptr;

    void makeMessageReadyForAdding(Message *message);
};

