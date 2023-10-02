#include "markdownutils.h"

using namespace Qt::Literals::StringLiterals;

auto MarkdownUtils::markdownToHtml(const QString &markdown) -> QString
{
#if MARKDOWN_LIBRARY == MARKDOWN_CMARK_GFM

    return markdownToHtmlCMark(markdown);

#elif MARKDOWN_LIBRARY == MARKDOWN_DISCOUNT

    return markdownToHtmlDiscount(markdown);

#else

    return u""_s;

#endif
}

#if MARKDOWN_LIBRARY == MARKDOWN_CMARK_GFM
auto MarkdownUtils::markdownToHtmlCMark(const QString &markdown) -> QString
{
    int options = CMARK_OPT_STRIKETHROUGH_DOUBLE_TILDE;

    cmark_gfm_core_extensions_ensure_registered();

    auto *parser = cmark_parser_new(options);
    addMarkdownExtension(parser, "strikethrough");
    addMarkdownExtension(parser, "table");
    addMarkdownExtension(parser, "autolink");
    addMarkdownExtension(parser, "tasklist");

    auto utf8 = markdown.toUtf8();
    cmark_parser_feed(parser, utf8, utf8.length());
    auto *doc = cmark_parser_finish(parser);
    cmark_parser_free(parser);

    auto html = QString::fromUtf8(cmark_render_html(doc, options, nullptr));
    cmark_node_free(doc);

    return html;
}

void MarkdownUtils::addMarkdownExtension(cmark_parser *parser, const QString &extension)
{
    auto *ext = cmark_find_syntax_extension(extension.toUtf8());
    if (ext) cmark_parser_attach_syntax_extension(parser, ext);
}

#elif MARKDOWN_LIBRARY == MARKDOWN_DISCOUNT
auto MarkdownUtils::markdownToHtmlDiscount(const QString &markdown) -> QString
{
    auto utf8 = markdown.toUtf8();

#ifdef MKD_NOLINKS
    // Discount v2
    auto flags = MKD_AUTOLINK;
    auto *mmiot = mkd_string(utf8, utf8.size(), flags);

    if (mmiot == nullptr) return u""_s;

    mkd_compile(mmiot, flags);
#else
    // Discount v3
    auto *flags = mkd_flags();
    mkd_set_flag_num(flags, MKD_AUTOLINK);
    auto *mmiot = mkd_string(utf8, utf8.size(), flags);

    if (mmiot == nullptr) return u""_s;

    mkd_compile(mmiot, flags);
#endif

    char *cBuff = nullptr;
    const int size = mkd_document(mmiot, &cBuff);

    QByteArray html(cBuff, size); // copy raw data
    mkd_cleanup(mmiot);

#ifndef MKD_NOLINKS
    mkd_free_flags(flags);
#endif

    return QString(html);
}
#endif
