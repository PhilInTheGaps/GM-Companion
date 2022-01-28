#pragma once

#include <QObject>
#include <QDateTime>
#include <QString>

#include "message.h"

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    MessageDispatcher(const QDateTime &timestamp, QtMsgType type, const QString &body);

signals:
    void dispatchMessage(Message *message);
};

