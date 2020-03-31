#ifndef AUDIOICONGENERATOR_H
#define AUDIOICONGENERATOR_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include <QNetworkAccessManager>
#include "audioproject.h"
#include "services/spotify.h"
#include <QReadWriteLock>

class IconWorker : public QObject
{
    Q_OBJECT

public:
    IconWorker(QString resourcesPath, QString musicPath, QString soundsPath);
    ~IconWorker();

public slots:
    void generateThumbnails(AudioScenario *scenario);
    void generateThumbnail(AudioElement *element);

private:
    QList<AudioScenario*> m_scenarios;
    QNetworkAccessManager *m_networkManager = nullptr;
    QString m_resourcesPath, m_musicPath, m_soundsPath;

    QPixmap getPlaceholderImage(AudioElement *element);

    QStringList m_spotifyIconList;
    QList<int> m_spotifyRequestList;

    void makeCollage(AudioElement *element);
    void loadImageFromWeb(AudioElement *element, const QString& url);
    void loadImageFromPath(AudioElement *element, const QString& filePath);
    QPixmap getImageFromAudioFile(AudioElement *element, AudioFile *audioFile);
    void getImagesFromSpotify();

    void insertImage(const QPixmap& image, const QString& uri);
    void insertImageFromSpotifyPlaylist(QJsonObject playlist);
    void insertImageFromSpotifyAlbum(QJsonObject album);
    void insertImageFromSpotifyTrack(QJsonObject track);
    void insertImageFromUrl(const QString& imageUrl, QString uri = "");

    void generateCollageImage(AudioElement *element);

    static QRectF getTargetRect(int imageWidth, int imageHeight, int imageCount, int index);
    static QRectF getSourceRect(QRect imageRect, int imageCount, int index);

private slots:
    void onSpotifyAuthorized();
    void onReceivedSpotifyReply(int id, QNetworkReply::NetworkError error, const QByteArray& data);

signals:
    void getSpotifyRequest(QNetworkRequest request, int requestId);
    void getFile(int requestId, QString filePath);
};

class AudioIconGenerator : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    static AudioIconGenerator* getInstance();
    ~AudioIconGenerator();

    static void generateIcons(AudioScenario *scenario);
    static void generateIcon(AudioElement *element);

    static void writeToCache(QUrl url, QPixmap pixmap);
    static QPixmap readFromCache(QUrl url);
    static bool cacheContains(QUrl url);
    static bool tryLoadFromCache(QUrl url, AudioElement *element);

    static QPixmap getPlaceholderImage(int type);

private:
    explicit AudioIconGenerator();
    static bool instanceFlag;
    static AudioIconGenerator *single;
    static QReadWriteLock cacheLock;
    static QMap<QUrl, QPixmap> iconCache;

    void _generateIcons(AudioScenario *scenario);
    void _generateIcon(AudioElement *element);

    IconWorker *m_worker = nullptr;

signals:
    void startGeneratingAll(AudioScenario *scenario);
    void startGeneratingOne(AudioElement* element);
};

#endif // AUDIOICONGENERATOR_H
