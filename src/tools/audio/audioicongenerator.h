#ifndef AUDIOICONGENERATOR_H
#define AUDIOICONGENERATOR_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QNetworkAccessManager>
#include <QQueue>
#include "audioproject.h"
#include "settings/settingsmanager.h"
#include "services/spotify.h"

class IconWorker : public QObject
{
    Q_OBJECT

public:
    IconWorker(QList<AudioProject*> projects);
    ~IconWorker();

public slots:
    void generate();
    void fetchedSpotifyIcon(QNetworkReply *reply, AudioElement *element);

private:
    SettingsManager sManager;
    QList<AudioProject*> m_projects;
    QNetworkAccessManager *m_networkManager = nullptr;

    static QMap<QUrl, QImage> iconCache;
    int m_currentSpotifyRequests = 0;
    QQueue<AudioElement*> m_queue;

    void generateCollage(AudioElement *element, int index = 0);
    void fetchSpotifyIcon(AudioElement *element, int index = 0);
    void startNextSpotifyRequest();
    void generateCollageImage(AudioElement *element);
    int addCollageIcon(AudioElement *element, QImage image);

    QRectF getTargetRect(int imageWidth, int imageHeight, int imageCount, int index);
    QRectF getSourceRect(QRect imageRect, int imageCount, int index);

private slots:
    void onSpotifyAuthorized();
};

class AudioIconGenerator : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    explicit AudioIconGenerator(QObject *parent = nullptr) {}
    ~AudioIconGenerator();

    void generateIcons(QList<AudioProject*> projects);

signals:
    void startGenerating();

};

#endif // AUDIOICONGENERATOR_H
