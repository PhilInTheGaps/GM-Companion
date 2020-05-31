#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

#include <QObject>
#include <QThread>

class HtmlWorker : public QObject
{
    Q_OBJECT
public:
    HtmlWorker(QObject *parent = nullptr) : QObject(parent) {}
    ~HtmlWorker() {}

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
    ~HtmlGenerator();

signals:
    void startGenerating(const QString& raw, int id);
    void generated(const QString &html, int id);

private:
    QThread m_workerThread;
    HtmlWorker *m_worker = nullptr;

};

#endif // HTMLGENERATOR_H
