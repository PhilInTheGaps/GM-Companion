#ifndef CHARACTERVIEWERTOOL_H
#define CHARACTERVIEWERTOOL_H

#include <QWidget>
#include <QListWidgetItem>

#include "managers/settingsmanager.h"

namespace Ui {
class CharacterViewerTool;
}

class CharacterViewerTool : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterViewerTool(QWidget *parent = 0);
    ~CharacterViewerTool();

private slots:
    void on_listWidget_characters_currentItemChanged(QListWidgetItem *current);

    void on_pushButton_newCharacter_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::CharacterViewerTool *ui;

    SettingsManager *settingsManager;

    void getCharacters();
};

#endif // CHARACTERVIEWERTOOL_H
