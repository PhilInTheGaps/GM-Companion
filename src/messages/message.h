#pragma once

#include <QDateTime>
#include <QString>

struct Message
{
    explicit Message(const QDateTime &timestamp, QtMsgType type, const QString &category, const QString &body)
        : timestamp(timestamp), type(type), category(category), body(body)
    {
    }

    explicit Message(QDateTime &&timestamp, QtMsgType type, QString &&category, QString &&body)
        : timestamp(std::move(timestamp)), type(type), category(std::move(category)), body(std::move(body))
    {
    }

    QDateTime timestamp;
    QtMsgType type;
    QString category;
    QString body;
};
