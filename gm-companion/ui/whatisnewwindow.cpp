#include "whatisnewwindow.h"
#include "ui_whatisnewwindow.h"

#include <QProcess>
#include <QDebug>

WhatIsNewWindow::WhatIsNewWindow(QWidget *parent) : QDialog(parent), ui(
        new Ui::WhatIsNewWindow)
{
    ui->setupUi(this);

    settingsManager = new SettingsManager;

    QSettings settings(QDir::homePath() + "/.gm-companion/settings.ini",
                       QSettings::IniFormat);
    int openNewWindow = settings.value("openWhatIsNewWindow", 1).toInt();

    if (openNewWindow == 0) ui->dontshowAgainCheckBox->setChecked(true);
}

WhatIsNewWindow::~WhatIsNewWindow()
{
    delete ui;
}

void WhatIsNewWindow::on_closeButton_clicked()
{
    this->close();
}

void WhatIsNewWindow::on_dontshowAgainCheckBox_toggled(bool checked)
{
    if (checked) {
        settingsManager->setSetting(Setting::openWhatIsNewWindow, false);
    } else {
        settingsManager->setSetting(Setting::openWhatIsNewWindow, true);
    }
}

void WhatIsNewWindow::on_nextButton_clicked()
{
    if (ui->stackedWidget->count() - 1 > ui->stackedWidget->currentIndex()) {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);

        if (ui->stackedWidget->count() - 1 ==
            ui->stackedWidget->currentIndex()) ui->nextButton->setDisabled(true);
    }
}

void WhatIsNewWindow::on_languageComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        settingsManager->setSetting(Setting::language, true, "en");
        break;

    case 1:
        settingsManager->setSetting(Setting::language, true, "de");
        break;

    default:
        settingsManager->setSetting(Setting::language, true, "en");
        break;
    }
}

void WhatIsNewWindow::on_buttonStyleComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        settingsManager->setSetting(Setting::buttonStyle, true, "small");
        break;

    case 1:
        settingsManager->setSetting(Setting::buttonStyle, true, "large");
        break;

    default:
        settingsManager->setSetting(Setting::buttonStyle,
                                    true,
                                    "small");
        break;
    }
}

void WhatIsNewWindow::on_pushButton_clicked()
{
    #ifdef __linux__
    QProcess::startDetached("java", { "-jar",
                                      "/usr/share/gm-companion/AddonManager.jar" });
    #elif _WIN32
    QProcess::startDetached("java",
                            { "-jar",
                              QApplication::applicationDirPath() +
                              "/AddonManager.jar" });
    #else // ifdef __linux__
    qDebug() << QCoreApplication::translate("",
                                            "This OS is not supported. Cannot launch Addon Manager.");
    #endif // ifdef __linux__
}
