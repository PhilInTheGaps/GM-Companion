#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QList>
#include <QCheckBox>

#include "mainwindow.h"
#include "managers/settingsmanager.h"

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog{
    Q_OBJECT

public:
    explicit OptionsDialog(MainWindow *parent = 0);
    ~OptionsDialog();

private slots:

    void onClose();
    void on_closeButton_clicked();

    // Paths
    void on_setMusicPath_clicked();
    void on_setSoundPath_clicked();
    void on_setResourcesPath_clicked();
    void on_setMapsPath_clicked();
    void on_setCharactersPath_clicked();
    void on_setNotesPath_clicked();
    void on_setAudioPath_clicked();
    void on_setRadioPath_clicked();

    // Addons
    void on_selectAll_clicked();
    void on_deselectAll_clicked();
    void on_addonManagerButton_clicked();

    // Stylesheet
    void on_styleComboBox_currentTextChanged(const QString &arg1);

    // Language
    void on_languageComboBox_currentIndexChanged(int index);

private:
    Ui::OptionsDialog *ui;

    MainWindow* w;

    SettingsManager* settings;

    bool pathsChanged = false;

    void updatePaths();

    void getAddons();

    void writeAddonSettings();

    QList<QCheckBox*> officialAddons;
    QList<QCheckBox*> inofficialAddons;
};

#endif // OPTIONSDIALOG_H
