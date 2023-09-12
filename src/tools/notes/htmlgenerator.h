#pragma once

#include <QObject>
#include <QThread>

class HtmlWorker : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

public slots:
    void generate(const QString &raw, int id);

signals:
    void generated(const QString &html, int id);
};

class HtmlGenerator : public QObject
{
    Q_OBJECT
public:
    explicit HtmlGenerator(QObject *parent = nullptr);
    ~HtmlGenerator() override;

signals:
    void startGenerating(const QString &raw, int id);
    void generated(const QString &html, int id);

private:
    QThread m_workerThread;
    HtmlWorker *m_worker = nullptr;
};
