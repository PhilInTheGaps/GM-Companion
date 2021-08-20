#pragma once

#include "fileresult.h"
#include <QByteArray>

namespace Files {

class FileDataResult : public FileResult
{
    Q_OBJECT

public:
    explicit FileDataResult(const QString& errorMessage, QObject *parent = nullptr)
        : FileResult(errorMessage, parent) {}

    explicit FileDataResult(const QByteArray& data, QObject *parent = nullptr)
        : FileResult(true, QByteArray(), parent), m_data(data) {}

    QByteArray data() const { return m_data; }

private:
    const QByteArray m_data;
};

}
