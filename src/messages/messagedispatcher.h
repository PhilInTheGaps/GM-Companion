#pragma once

#include "message.h"
#include <QDateTime>
#include <QObject>
#include <QString>

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