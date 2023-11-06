#pragma once

#include "fileresult.h"
#include <QByteArray>
#include <utility>

namespace Files
{

class FileDataResult : public FileResult
{
public:
    using FileResult::FileResult;

    explicit FileDataResult(const QByteArray &data) : FileResult(true, QByteArray()), m_data(data)
    {
    }

    explicit FileDataResult(QByteArray &&data) : FileResult(true, QByteArray()), m_data(std::move(data))
    {
    }

    static auto fromRestReply(const Services::RestReply &reply) -> FileDataResult;

    [[nodiscard]] auto data() const -> const QByteArray &
    {
        return m_data;
    }

private:
    QByteArray m_data;
};

} // namespace Files
