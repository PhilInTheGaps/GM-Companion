#include "htmlgenerator.h"
#include "utils/markdownutils.h"
#include <QtConcurrent>

using namespace Qt::Literals::StringLiterals;

auto HtmlGenerator::generateFromMarkdownAsync(const QString &markdown) -> QFuture<QString>
{
    return QtConcurrent::run(generateFromMarkdown, markdown);
}

auto HtmlGenerator::generateFromMarkdown(const QString &markdown) -> QString
{
    auto html = MarkdownUtils::markdownToHtml(markdown);

    // Qt RichText can only display <s>, not <del> for strikethrough
    html.replace("<del>"_L1, "<s>"_L1).replace("</del>"_L1, "</s>"_L1);

    // Insert table style manually here, does not work via CSS
    html.replace("<table>"_L1, R"(<table border="1" cellspacing="0" cellpadding="10">)"_L1);

    return html;
}
