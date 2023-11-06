#include "markdownhighlighter.h"
#include <QRegularExpression>

static constexpr auto MD_HEADER_REGEX = R"(^#{1,6}\s.+$)";

static constexpr auto MD_BOLD_REGEX = R"(\*{2}[^\s].*[^\s]\*{2})";
static constexpr auto MD_BOLD_ALT_REGEX = R"(_{2}[^\s].*[^\s]_{2})";

static constexpr auto MD_ITALIC_REGEX = R"((^|[^\*])\*([^\s\*].*[^\s\*])\*)";
static constexpr auto MD_ITALIC_ALT_REGEX = R"((^|[^\_])\_([^\s\_].*[^\s\_])\_)";

static constexpr auto MD_CODE_INLINE_REGEX = R"(\`.*\`)";
static constexpr auto MD_STRIKETHROUGH_REGEX = R"(~{2}(.*)~{2})";
static constexpr auto MD_QUOTE_REGEX = R"(^\>\s)";

static constexpr auto MD_LIST_ASTERISK_REGEX = R"(^\s*\*\s)";
static constexpr auto MD_LIST_DASH_REGEX = R"(^\s*\-\s)";
static constexpr auto MD_LIST_PLUS_REGEX = R"(^\s*\+\s)";
static constexpr auto MD_LIST_DIGIT_REGEX = R"(^\s*\d+\.\s)";

static constexpr auto HEADER_COLOR = QColor(0x56, 0x9c, 0xd6);
static constexpr auto CODE_COLOR = QColor(0xe6, 0xa1, 0x52);
static constexpr auto QUOTE_COLOR = QColor(0x56, 0x9c, 0xd6);
static constexpr auto LIST_COLOR = QColor(0x56, 0x9c, 0xd6);

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

void MarkdownHighlighter::applyRegexMatch(const QString &text, const QString &regex, const QTextCharFormat &charFormat,
                                          int group)
{
    const QRegularExpression expression(regex);
    auto iterator = expression.globalMatch(text);

    while (iterator.hasNext())
    {
        auto match = iterator.next();

        for (auto i = static_cast<int>(match.capturedStart(group)); i < match.capturedEnd(group); i++)
        {
            setFormat(i, 1, combineFormats(format(i), charFormat));
        }
    }
}

auto MarkdownHighlighter::combineFormats(const QTextCharFormat &one, const QTextCharFormat &two) -> QTextCharFormat
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

    if (auto isTypeWriter = one.fontStyleHint() == QFont::TypeWriter || two.fontStyleHint() == QFont::TypeWriter;
        isTypeWriter)
        format.setFontStyleHint(QFont::TypeWriter);

    return format;
}

auto MarkdownHighlighter::headerFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(HEADER_COLOR);
    return format;
}

auto MarkdownHighlighter::boldFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    return format;
}

auto MarkdownHighlighter::italicFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setFontItalic(true);
    return format;
}

auto MarkdownHighlighter::codeInlineFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setFontStyleHint(QFont::TypeWriter);
    format.setForeground(CODE_COLOR);
    return format;
}

auto MarkdownHighlighter::strikethroughFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setFontStrikeOut(true);
    return format;
}

auto MarkdownHighlighter::quoteFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setForeground(QUOTE_COLOR);
    return format;
}

auto MarkdownHighlighter::listFormat() -> QTextCharFormat
{
    QTextCharFormat format;
    format.setForeground(LIST_COLOR);
    return format;
}
