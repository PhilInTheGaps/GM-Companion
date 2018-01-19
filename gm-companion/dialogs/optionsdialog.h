#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QList>
#include <QCheckBox>

#include "gm-companion/mainwindow.h"
#include "gm-companion/managers/settingsmanager.h"

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog{
    Q_OBJECT

public:
    explicit OptionsDialog(MainWindow *parent = 0);
    ~OptionsDialog();

private slots:
    // Paths
    void on_setMusicPath_clicked();
    void on_setSoundPath_clicked();
    void on_setResourcesPath_clicked();
    void on_setMapsPath_clicked();
    void on_setCharactersPath_clicked();
    void on_setNotesPath_clicked();
    void on_setAudioPath_clicked();
    void on_setRadioPath_clicked();
    void on_setShopPath_clicked();

    // Addons
    void on_selectAll_clicked();
    void on_deselectAll_clicked();
    void on_addonManagerButton_clicked();
    void on_pushButton_saveAddons_clicked();

    // Stylesheet
    void on_styleComboBox_currentTextChanged(const QString &arg1);

    // Language
    void on_languageComboBox_currentIndexChanged(int index);

    // Sidebar
    void on_pushButton_openWiki_clicked();
    void on_pushButton_reportABug_clicked();
    void on_pushButton_downloadOlderVersions_clicked();
    void on_pushButton_viewOnGitHub_clicked();
    void on_pushButton_about_clicked();
    void on_pushButton_checkForUpdates_clicked();

    void on_checkBox_showToolNames_toggled(bool checked);

private:
    Ui::OptionsDialog *ui;

    MainWindow* w;

    SettingsManager* settings;

    bool pathsChanged = false;
    void updatePaths();
    void setPath(Setting setting);

    void getAddons();
    void getStyleSheets();
    void getLanguage();

    void writeAddonSettings();

    QList<QCheckBox*> officialAddons;
    QList<QCheckBox*> inofficialAddons;
};

#endif // OPTIONSDIALOG_H
