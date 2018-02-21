#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include <QWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <QDir>

#include "gm-companion/settings/settingsmanager.h"
#include "sheets/defaultsheet.h"
#include "sheets/dsa5sheet.h"
#include "sheets/entaria2sheet.h"
#include "sheets/dnd5esheet.h"

namespace Ui {
class CharacterTool;
}

class CharacterTool : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterTool(QWidget *parent = 0);
    ~CharacterTool();

private slots:
    void on_pushButton_makeInactive_clicked();
    void on_pushButton_makeAllInactive_clicked();

    void on_pushButton_makeActive_clicked();
    void on_pushButton_makeAllActive_clicked();

    void on_pushButton_createNewCharacter_clicked();

    void on_listWidget_activeCharacters_currentItemChanged(QListWidgetItem *item);

    void on_pushButton_save_clicked();
    void on_pushButton_delete_clicked();

    void on_listWidget_pages_currentItemChanged(QListWidgetItem *item);
    void on_pushButton_zoomIn_clicked();
    void on_pushButton_zoomOut_clicked();
    void on_pushButton_reset_clicked();

private:
    Ui::CharacterTool *ui;

    SettingsManager *settingsManager;

    DefaultSheet *defaultSheet;
    DnD5eSheet *dnd5eSheet;
    DSA5Sheet *dsa5Sheet;
    Entaria2Sheet *entaria2Sheet;

    QStringList inactiveCharacters;
    QString currentCharacter;

    void setTemplates();
    void loadCharacterSheets();
    void updateCharacterList();

    void loadCharacterImages(QString path, QStringList files);

    void createNewCharacter();

    void makeCharacterInactive(int index);
    void makeCharacterActive(int index);
};

#endif // CHARACTERTOOL_H
