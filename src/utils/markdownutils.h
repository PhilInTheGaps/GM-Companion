#ifndef MARKDOWNUTILS_H
#define MARKDOWNUTILS_H

#include <QString>
#include <cmark-gfm.h>

class MarkdownUtils
{
public:
    static QString markdownToHtml(const QString &markdown);

private:
    static void addMarkdownExtension(cmark_parser *parser, const QString &extension);
};

#endif // MARKDOWNUTILS_H
