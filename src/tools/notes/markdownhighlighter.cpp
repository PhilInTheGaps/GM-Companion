#include "markdownhighlighter.h"
#include "utils/utils.h"
#include <QRegularExpression>

#define MD_HEADER_REGEX R"(^#{1,6}\s.+$)"

#define MD_BOLD_REGEX R"(\*{2}[^\s].*[^\s]\*{2})"
#define MD_BOLD_ALT_REGEX R"(_{2}[^\s].*[^\s]_{2})"

#define MD_ITALIC_REGEX R"((^|[^\*])\*([^\s\*].*[^\s\*])\*)"
#define MD_ITALIC_ALT_REGEX R"((^|[^\_])\_([^\s\_].*[^\s\_])\_)"

#define MD_CODE_INLINE_REGEX R"(\`.*\`)"
#define MD_STRIKETHROUGH_REGEX R"(~{2}(.*)~{2})"
#define MD_QUOTE_REGEX R"(^\>\s)"

#define MD_LIST_ASTERISK_REGEX R"(^\s*\*\s)"
#define MD_LIST_DASH_REGEX R"(^\s*\-\s)"
#define MD_LIST_PLUS_REGEX R"(^\s*\+\s)"
#define MD_LIST_DIGIT_REGEX R"(^\s*\d+\.\s)"

MarkdownHighlighter::MarkdownHighlighter(QObject *parent)
    : QSyntaxHighlighter(parent)
{

}

void MarkdownHighlighter::highlightBlock(const QString &text)
{   
    // Headers
    applyRegexMatch(text, MD_HEADER_REGEX, headerFormat());

    // Italic
    applyRegexMatch(text, MD_ITALIC_REGEX, italicFormat(), 2);
    applyRegexMatch(text, MD_ITALIC_ALT_REGEX, italicFormat(), 2);

    // Bold
    applyRegexMatch(text, MD_BOLD_REGEX, boldFormat());
    applyRegexMatch(text, MD_BOLD_ALT_REGEX, boldFormat());

    // Code
    applyRegexMatch(text, MD_CODE_INLINE_REGEX, codeInlineFormat());

    // Strikethrough
    applyRegexMatch(text, MD_STRIKETHROUGH_REGEX, strikethroughFormat(), 1);

    // Quotes
    applyRegexMatch(text, MD_QUOTE_REGEX, quoteFormat());

    // Lists
    applyRegexMatch(text, MD_LIST_ASTERISK_REGEX, listFormat());
    applyRegexMatch(text, MD_LIST_DASH_REGEX, listFormat());
    applyRegexMatch(text, MD_LIST_PLUS_REGEX, listFormat());
    applyRegexMatch(text, MD_LIST_DIGIT_REGEX, listFormat());
}

void MarkdownHighlighter::applyRegexMatch(const QString &text, const QString &regex, const QTextCharFormat &charFormat, int group)
{
    QRegularExpression expression(regex);
    auto iterator = expression.globalMatch(text);

    while (iterator.hasNext())
    {
      auto match = iterator.next();

      for (int i = match.capturedStart(group); i < match.capturedEnd(group); i++)
      {
          setFormat(i, 1, combineFormats(format(i), charFormat));
      }
    }
}

QTextCharFormat MarkdownHighlighter::combineFormats(const QTextCharFormat &one, const QTextCharFormat &two)
{
    QTextCharFormat format;
    format.setFontWeight(qMax(one.fontWeight(), two.fontWeight()));
    format.setFontItalic(one.fontItalic() || two.fontItalic());
    format.setFontStrikeOut(one.fontStrikeOut() || two.fontStrikeOut());

    if (format.foreground() != two.foreground())
    {
        format.setForeground(two.foreground());
    }
    else if (format.foreground() != one.foreground())
    {
        format.setForeground(one.foreground());
    }

    auto isTypeWriter = one.fontStyleHint() == QFont::TypeWriter
            || two.fontStyleHint() == QFont::TypeWriter;

    if (isTypeWriter) format.setFontStyleHint(QFont::TypeWriter);

    return format;
}

QTextCharFormat MarkdownHighlighter::headerFormat()
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(QColor("#569cd6"));
    return format;
}

QTextCharFormat MarkdownHighlighter::boldFormat()
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    return format;
}

QTextCharFormat MarkdownHighlighter::italicFormat()
{
    QTextCharFormat format;
    format.setFontItalic(true);
    return format;
}

QTextCharFormat MarkdownHighlighter::codeInlineFormat()
{
    QTextCharFormat format;
    format.setFontStyleHint(QFont::TypeWriter);
    format.setForeground(QColor("#e6a152"));
    return format;
}

QTextCharFormat MarkdownHighlighter::strikethroughFormat()
{
    QTextCharFormat format;
    format.setFontStrikeOut(true);
    return format;
}

QTextCharFormat MarkdownHighlighter::quoteFormat()
{
    QTextCharFormat format;
    format.setForeground(QColor("#569cd6"));
    return format;
}

QTextCharFormat MarkdownHighlighter::listFormat()
{
    QTextCharFormat format;
    format.setForeground(QColor("#569cd6"));
    return format;
}
