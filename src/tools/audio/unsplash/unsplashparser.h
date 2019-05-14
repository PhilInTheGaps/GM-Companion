#ifndef UNSPLASHPARSER_H
#define UNSPLASHPARSER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QJsonDocument>
#include <QNetworkAccessManager>

#include "unsplashimage.h"

class UnsplashParser : public QObject
{
    Q_OBJECT
public:
    explicit UnsplashParser(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    void findImage(QString text);
    void shuffle();

private:
    QQmlApplicationEngine *qmlEngine;
    ImageListModel *m_imageModel;
    QList<UnsplashImage *> m_images;
    QNetworkAccessManager *m_manager;

    QJsonDocument m_doc;

    void parse();
};

#endif // UNSPLASHPARSER_H
