#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QWidget>
#include <QStringList>
#include <QFrame>
#include <QSettings>
#include <QSignalMapper>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QKeyEvent>
#include <QList>

#include "gm-companion/managers/settingsmanager.h"

struct Song {
    QString title;
    QString path;
};

namespace Ui {
class AudioTool;
}

class AudioTool : public QWidget
{
    Q_OBJECT

public:
    explicit AudioTool(SettingsManager *sManager, QWidget *parent = 0);
    ~AudioTool();

private slots:

    // Music
    void updateMetaData();
    void on_listWidget_songs_currentRowChanged(int currentRow);

    void on_pushButton_play_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_replay_clicked();
    void on_pushButton_next_clicked();

    // Change volume
    void on_horizontalSlider_music_valueChanged(int value);
    void on_horizontalSlider_sound_valueChanged(int value);

    void on_listWidget_scenarios_currentRowChanged(int currentRow);

    void on_pushButton_openEditor_clicked();
    void on_checkBox_setProjectAsDefault_toggled(bool checked);
    void on_pushButton_loadProject_clicked();
    void on_comboBox_projects_currentTextChanged(const QString &arg1);

    void on_pushButton_updateProjects_clicked();
    void on_pushButton_documentation_clicked();

private:
    Ui::AudioTool *ui;

    SettingsManager *settingsManager;

    void keyPressEvent(QKeyEvent *event);

    // Project
    QString currentProject;
    void getProjects();
    void loadProject(QString project);

    // Music
    void playMusic(QString arg);
    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;

    // Radio
    void playRadio(QString arg);
    QMediaPlayer *radioPlayer;
    bool radioActive;

    // Sound
    void playSound(QString arg);
    QList<QMediaPlayer*> soundPlayerList;

    // List View
    void generateScenarioList(QString category); // Scenarios
    void generateElementButtons(QString scenario); // Elements

    // Categories
    void changeCategory(QString category);
    QSignalMapper *signalMapperCategories;
    QString currentCategory;
};

#endif // AUDIOTOOL_H
