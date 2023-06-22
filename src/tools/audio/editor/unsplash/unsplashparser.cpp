#include "unsplashparser.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQmlContext>
#include <algorithm>
#include <random>

Q_LOGGING_CATEGORY(gmUnsplashParser, "gm.unsplash.parser")

UnsplashParser::UnsplashParser(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent)
{
    qmlEngine = engine;
    m_imageModel = new ImageListModel;
    m_manager = new QNetworkAccessManager;
    qmlEngine->rootContext()->setContextProperty("unsplashImageListModel", m_imageModel);
    parse();
}

void UnsplashParser::parse()
{
    QNetworkRequest request(QUrl("https://gist.githubusercontent.com/PhilInTheGaps/0d61b30b0f5ccc00f4abd81566a4cf77/"
                                 "raw/gm-companion-unsplash.json"));

    auto reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        qCDebug(gmUnsplashParser()) << "Received Unsplash JSON file.";

        if (reply->error() != QNetworkReply::NoError)
        {
            qCDebug(gmUnsplashParser()) << "Unsplash JSON Error:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        m_doc = QJsonDocument::fromJson(reply->readAll());

        auto array = m_doc.array();

        for (auto o : array)
        {
            auto *image = new UnsplashImage;
            image->setId(o.toObject().value("id").toString());
            image->setAuthor(o.toObject().value("author").toString());
            image->setAuthorId(o.toObject().value("author_id").toString());

            QStringList tags;

            for (auto t : o.toObject().value("tags").toArray())
            {
                tags.append(t.toString());
            }

            image->setTags(tags);

            m_images.append(image);
        }

        m_imageModel->setElements(m_images);

        reply->deleteLater();
    });
}

void UnsplashParser::findImage(const QString &text)
{
    qCDebug(gmUnsplashParser()) << "Searching for" << text;

    if (text.isEmpty())
    {
        m_imageModel->setElements(m_images);
        return;
    }

    QList<UnsplashImage *> images;
    QStringList terms = text.split(" ");

    for (auto i : m_images)
    {
        for (const auto &term : terms)
        {
            if (images.contains(i)) break;

            for (const auto &tag : i->tags())
            {
                if (images.contains(i)) break;

                if (tag.contains(term, Qt::CaseInsensitive))
                {
                    images.append(i);
                    break;
                }
            }
        }
    }

    m_imageModel->setElements(images);
}

void UnsplashParser::shuffle()
{
    auto images = m_images;

    std::shuffle(images.begin(), images.end(), std::mt19937(std::random_device()()));

    m_imageModel->setElements(images);
}
