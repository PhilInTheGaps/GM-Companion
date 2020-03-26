#ifndef AUDIOICONGENERATOR_H
#define AUDIOICONGENERATOR_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QNetworkAccessManager>
#include <QQueue>
#include "audioproject.h"
#include "services/spotify.h"

class IconWorker : public QObject
{
    Q_OBJECT

public:
    IconWorker(QList<AudioProject*> projects);
    ~IconWorker();

public slots:
    void generateThumbnails();

private:
    QList<AudioProject*> m_projects;
    QNetworkAccessManager *m_networkManager = nullptr;

    static QMap<QUrl, QImage> iconCache;

    QImage m_musicPlaceholderImage;
    QImage m_soundPlaceholderImage;
    QImage getPlaceholderImage(AudioElement *element);
    void loadPlaceholderImages();

    QStringList m_spotifyIconList;
    QList<int> m_spotifyRequestList;
    void makeThumbnail(AudioElement *element);
    QImage getImageFromAudioFile(AudioElement *element, AudioFile *audioFile);
    void getImagesFromSpotify();

    void insertImage(QImage image, QString uri);
    void insertImageFromSpotifyPlaylist(QJsonObject playlist);
    void insertImageFromSpotifyAlbum(QJsonObject album);
    void insertImageFromSpotifyTrack(QJsonObject track);
    void insertImageFromUrl(QString imageUrl, QString uri = "");

    void generateCollageImage(AudioElement *element);

    QRectF getTargetRect(int imageWidth, int imageHeight, int imageCount, int index);
    QRectF getSourceRect(QRect imageRect, int imageCount, int index);

private slots:
    void onSpotifyAuthorized();
    void onReceivedSpotifyReply(int id, QNetworkReply::NetworkError error, QByteArray data);

signals:
    void getSpotifyRequest(QNetworkRequest request, int requestId);
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
