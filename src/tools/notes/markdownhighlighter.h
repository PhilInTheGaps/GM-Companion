#ifndef MARKDOWNHIGHLIGHTER_H
#define MARKDOWNHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>

class MarkdownHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit MarkdownHighlighter(QTextDocument *document);

protected:
    void highlightBlock(const QString &text) override;

private:
    void applyRegexMatch(const QString &text, const QString &regex, const QTextCharFormat &format, int group = 0);

    static QTextCharFormat combineFormats(const QTextCharFormat &one, const QTextCharFormat &two);

    static QTextCharFormat headerFormat();
    static QTextCharFormat boldFormat();
    static QTextCharFormat italicFormat();
    static QTextCharFormat codeInlineFormat();
    static QTextCharFormat strikethroughFormat();
    static QTextCharFormat quoteFormat();
    static QTextCharFormat listFormat();
};

#endif // MARKDOWNHIGHLIGHTER_H
