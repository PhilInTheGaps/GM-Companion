#ifndef AUDIOICONGENERATOR_H
#define AUDIOICONGENERATOR_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QReadWriteLock>
#include "project/audioproject.h"
#include "services/spotify/spotify.h"

class IconWorker : public QObject
{
    Q_OBJECT

public:
    IconWorker(const QString &musicPath, const QString &soundsPath);
    ~IconWorker();

public slots:
    void generateThumbnails(AudioScenario *scenario);
    void generateThumbnail(AudioElement *element);
    void onReceivedSpotifyReply(RestNetworkReply *reply);

signals:
    void sendSpotifyRequest(const QUrl &url);

private:
    QList<AudioScenario*> m_scenarios;
    QNetworkAccessManager *m_networkManager = nullptr;
    QString m_musicPath, m_soundsPath;

    QPixmap getPlaceholderImage(AudioElement *element);

    QStringList m_spotifyIconList;

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

    static void writeToCache(const QUrl &url, const QPixmap& pixmap);
    static QPixmap readFromCache(const QUrl& url);
    static bool cacheContains(const QUrl& url);
    static bool tryLoadFromCache(const QUrl& url, AudioElement *element);

    static QPixmap getPlaceholderImage(AudioElement::Type type);

private:
    explicit AudioIconGenerator(QObject* parent = nullptr);
    inline static AudioIconGenerator *single = nullptr;
    static QReadWriteLock cacheLock;
    inline static QMap<QUrl, QPixmap> iconCache;

    void _generateIcons(AudioScenario *scenario);
    void _generateIcon(AudioElement *element);

    IconWorker *m_worker = nullptr;

signals:
    void startGeneratingAll(AudioScenario *scenario);
    void startGeneratingOne(AudioElement *element);
    void receivedSpotifyReply(RestNetworkReply *reply);
};

#endif // AUDIOICONGENERATOR_H
