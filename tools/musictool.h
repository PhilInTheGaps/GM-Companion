#ifndef MUSICTOOL_H
#define MUSICTOOL_H

#include <QWidget>
#include "settingsmanager.h"

namespace Ui {
class MusicTool;
}

class MusicTool : public QWidget
{
    Q_OBJECT

public:
    explicit MusicTool(QWidget *parent = 0);
    ~MusicTool();

private slots:
    void on_radioButton_List_clicked();

    void on_radioButton_Large_clicked();

    void on_radioButton_Small_clicked();

private:
    Ui::MusicTool *ui;

    void generateButtons();

    SettingsManager *settingsManager;

};

#endif // MUSICTOOL_H
