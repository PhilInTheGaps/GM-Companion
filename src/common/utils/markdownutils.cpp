#include "markdownutils.h"

QString MarkdownUtils::markdownToHtml(const QString &markdown)
{
    #if MARKDOWN_LIBRARY == MARKDOWN_CMARK_GFM
    
    return markdownToHtmlCMark(markdown);

    #elif MARKDOWN_LIBRARY == MARKDOWN_DISCOUNT
    
    return markdownToHtmlDiscount(markdown);

    #else

    return "";

    #endif
}

#if MARKDOWN_LIBRARY == MARKDOWN_CMARK_GFM
QString MarkdownUtils::markdownToHtmlCMark(const QString &markdown)
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

    auto html = QString::fromUtf8(cmark_render_html(doc, options, NULL));
    cmark_node_free(doc);

    return html;
}

void MarkdownUtils::addMarkdownExtension(cmark_parser *parser, const QString &extension)
{
    auto *ext = cmark_find_syntax_extension(extension.toUtf8());
    if (ext) cmark_parser_attach_syntax_extension(parser, ext);
}

#elif MARKDOWN_LIBRARY == MARKDOWN_DISCOUNT
QString MarkdownUtils::markdownToHtmlDiscount(const QString &markdown)
{
    auto markdownStd = markdown.toStdString();
    auto flags = MKD_AUTOLINK;
    MMIOT* doc = mkd_string(markdownStd.c_str(), markdownStd.size(), flags);

	if(doc == nullptr) return QStringLiteral("");
	
    mkd_compile(doc, flags);

	char *cstr_buff;
	int size = mkd_document(doc, &cstr_buff);

    std::string html;
	html.assign(cstr_buff, size);
	mkd_cleanup(doc);

    return QString::fromStdString(html);
}
#endif