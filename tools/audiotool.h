#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QWidget>
#include <QStringList>
#include <QFrame>
#include <QSettings>
#include <QSignalMapper>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "managers/settingsmanager.h"

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
    // Changing view
    void on_radioButton_List_clicked();
    void on_radioButton_Large_clicked();
    void on_radioButton_Small_clicked();

    // Music
    void playMusic(QString musicList);
    void on_comboBox_music_currentIndexChanged(int index);
    void updateMetaData();
    void on_listWidget_songs_currentRowChanged(int currentRow);

    void on_pushButton_play_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_replay_clicked();
    void on_pushButton_next_clicked();

    // Sound
    void playSound(QString soundList);

    // Change volume
    void on_horizontalSlider_music_valueChanged(int value);
    void on_horizontalSlider_sound_valueChanged(int value);

    void on_listWidget_categories_currentRowChanged(int currentRow);
    void on_listWidget_scenarios_currentRowChanged(int currentRow);

    void on_pushButton_openEditor_clicked();
    void on_checkBox_setProjectAsDefault_toggled(bool checked);
    void on_pushButton_loadProject_clicked();
    void on_comboBox_projects_currentTextChanged(const QString &arg1);

private:
    Ui::AudioTool *ui;

    SettingsManager *settingsManager;

    // Project
    QString currentProject;
    void getProjects();
    void loadProject(QString project);

    // Music
    QSignalMapper *signalMapperMusic;
    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;

    // Sound
    QSignalMapper *signalMapperSound;
    QList<QMediaPlayer*> soundPlayerList;

    QList<QFrame *> createGroupFrame(QString folder, QFrame *frame);

    void generateSmallButtons(QStringList categories);
    void generateCategoryList(QStringList categories);
    void generateScenarioList(QString category);

    void generateElementButtons(QString scenario);



};

#endif // AUDIOTOOL_H
