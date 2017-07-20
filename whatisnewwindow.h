#ifndef WHATISNEWWINDOW_H
#define WHATISNEWWINDOW_H

#include <QDialog>

#include "managers/settingsmanager.h"

namespace Ui {
class WhatIsNewWindow;
}

class WhatIsNewWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WhatIsNewWindow(QWidget *parent = 0);
    ~WhatIsNewWindow();

private slots:
    void on_closeButton_clicked();

    void on_dontshowAgainCheckBox_toggled(bool checked);

    void on_nextButton_clicked();

    void on_languageComboBox_currentIndexChanged(int index);

    void on_buttonStyleComboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::WhatIsNewWindow *ui;

    SettingsManager* settingsManager;
};

#endif // WHATISNEWWINDOW_H
