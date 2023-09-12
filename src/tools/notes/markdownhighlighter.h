#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>

class MarkdownHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    using QSyntaxHighlighter::QSyntaxHighlighter;

protected:
    void highlightBlock(const QString &text) override;

private:
    void applyRegexMatch(const QString &text, const QString &regex, const QTextCharFormat &format, int group = 0);

    static auto combineFormats(const QTextCharFormat &one, const QTextCharFormat &two) -> QTextCharFormat;

    static auto headerFormat() -> QTextCharFormat;
    static auto boldFormat() -> QTextCharFormat;
    static auto italicFormat() -> QTextCharFormat;
    static auto codeInlineFormat() -> QTextCharFormat;
    static auto strikethroughFormat() -> QTextCharFormat;
    static auto quoteFormat() -> QTextCharFormat;
    static auto listFormat() -> QTextCharFormat;
};
