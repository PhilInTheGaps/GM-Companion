#pragma once

#include <QDateTime>
#include <QObject>
#include <QString>

#include "message.h"

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    static void dispatch(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body);

private:
    using QObject::QObject;
    void dispatch(Message *message);

signals:
    void dispatchMessage(Message *message);
};
