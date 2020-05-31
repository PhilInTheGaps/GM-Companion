#include "htmlgenerator.h"
#include "utils/markdownutils.h"
#include <QDebug>

HtmlGenerator::HtmlGenerator(QObject *parent) : QObject(parent)
{
    m_worker = new HtmlWorker;
    m_worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &QObject::destroyed, &m_workerThread, &QThread::quit);

    connect(this, &HtmlGenerator::startGenerating, m_worker, &HtmlWorker::generate);
    connect(m_worker, &HtmlWorker::generated, this, &HtmlGenerator::generated);

    m_workerThread.start();
}

HtmlGenerator::~HtmlGenerator()
{
    m_worker->deleteLater();
    m_workerThread.quit();
    m_workerThread.wait();
}

void HtmlWorker::generate(const QString &raw, int id)
{
    auto html = MarkdownUtils::markdownToHtml(raw);

    // Qt RichText can only display <s>, not <del> for strikethrough
    html.replace("<del>", "<s>").replace("</del>", "</s>");

    // Insert table style manually here, does not work via CSS
    html.replace("<table>", "<table border=\"1\" cellspacing=\"0\" cellpadding=\"10\">");

    emit generated(html, id);
}
