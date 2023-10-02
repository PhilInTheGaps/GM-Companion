#pragma once

#include <QFuture>
#include <QObject>
#include <QString>

class HtmlGenerator
{
public:
    [[nodiscard]] static auto generateFromMarkdownAsync(const QString &markdown) -> QFuture<QString>;
    [[nodiscard]] static auto generateFromMarkdown(const QString &markdown) -> QString;
};
