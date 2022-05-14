#pragma once

#include <QObject>
#include <QDateTime>
#include <QString>

#include "message.h"

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    static void dispatch(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body);

private:
    MessageDispatcher();
    void dispatch(Message *message);

signals:
    void dispatchMessage(Message *message);
};

