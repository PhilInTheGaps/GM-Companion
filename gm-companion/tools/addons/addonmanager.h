#ifndef ADDONMANAGER_H
#define ADDONMANAGER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "gm-companion/settings/settingsmanager.h"

namespace Ui {
class AddonManager;
}

class AddonManager : public QWidget
{
    Q_OBJECT

public:
    explicit AddonManager(QWidget *parent = 0);
    ~AddonManager();

private slots:
    void fileDownloaded(QNetworkReply* reply);

    void on_pushButton_documentation_clicked();

private:
    Ui::AddonManager *ui;

    SettingsManager *settingsManager;

    void downloadFile(QUrl url);
    void downloadAddon(QUrl url);
    void localTest();
    void setUpTable();
    QString getStatus(QString addon, QString version);

    QNetworkAccessManager networkManager;
    QByteArray downloadedData;
};

#endif // ADDONMANAGER_H
