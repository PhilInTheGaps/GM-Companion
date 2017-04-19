#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMediaPlayer>
#include <QMediaPlayerControl>
#include <QMediaPlaylist>
#include <QUrl>
#include <QSignalMapper>
#include <QTreeWidget>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url, bool music);

private slots:
    void playMusic(QString);

    void playSound(QString);

    void updateMetaData();

    void on_musicPauseButton_clicked();

    void on_musicPlayButton_clicked();

    void on_musicReplayButton_clicked();

    void on_musicNextButton_clicked();

    void on_setMusicFolder_clicked();

    void on_setSoundFolder_clicked();

    void on_setMapsFolder_clicked();

    void on_setResourcesFolder_clicked();

    void on_tableDoubleClicked(int,int);

    void on_soundPlayButton_clicked();

    void on_soundPauseButton_clicked();

    void on_soundReplayButton_clicked();

    void on_soundNextButton_clicked();

private:
    Ui::MainWindow *ui;

    QString musicPath;
    QString soundPath;
    QString mapsPath;
    QString resourcesPath;

    QSignalMapper *signalMapperMusic;
    QSignalMapper *signalMapperSound;

    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;
    QMediaPlayer *metaPlayer;

    QTableWidget *musicTable;
    QTreeWidgetItem *folderItem;

    QMediaPlayer *soundPlayer;
    QMediaPlaylist *soundPlaylist;
};

#endif // MAINWINDOW_H
