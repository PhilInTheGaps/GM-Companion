#pragma once

#include "unsplashimage.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class UnsplashParser : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ImageListModel *model READ model CONSTANT)

public:
    explicit UnsplashParser(const QQmlEngine *engine, QObject *parent = nullptr);

    [[nodiscard]] auto model() -> ImageListModel *;

    void findImage(const QString &text);
    void shuffle();

private:
    ImageListModel m_imageModel;
    QList<UnsplashImage *> m_images;
    QNetworkAccessManager *m_networkManager;

    QJsonDocument m_doc;

    void parse();
};
