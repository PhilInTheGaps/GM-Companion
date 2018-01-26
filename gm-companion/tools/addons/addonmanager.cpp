#include "addonmanager.h"
#include "ui_addonmanager.h"

#include <QTextEdit>
#include <QFile>
#include <QDir>
#include <QTableWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDesktopServices>

AddonManager::AddonManager(QWidget *parent) : QWidget(parent), ui(
        new Ui::AddonManager)
{
    ui->setupUi(this);

    connect(&networkManager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(fileDownloaded(QNetworkReply *)));

    // Hide stuff on start
    ui->tableWidget->hide();
    ui->textBrowser->hide();

    settingsManager = new SettingsManager;

    downloadFile(QUrl(
                     "https://github.com/PhilInTheGaps/GM-Companion/raw/master/gm-companion/Addons/versions.txt"));

    //  localTest();
}

AddonManager::~AddonManager()
{
    delete ui;
}

// Sets up the addon table
void AddonManager::setUpTable()
{
    // Hide progress bar and show other stuff
    ui->progressBar->hide();
    ui->tableWidget->show();
    ui->textBrowser->show();
    ui->textBrowser->append(downloadedData);

    qDebug() << "Loading addons ...";

    QList<QByteArray> list = downloadedData.split(';');
    ui->tableWidget->setRowCount(list.size());

    for (int i = 0; i < list.size(); i++)
    {
        QByteArray s            = list.at(i);
        QList<QByteArray> local = s.split(':');

        // Add Status Button
        QString addonName = local.at(0);
        addonName = addonName.replace("\n", "");
        QString addonVersion = local.at(1);
        QString status       = getStatus(addonName, addonVersion);

        qDebug().noquote() << "   Loading addon:" << addonName;

        for (int j = 0; j < local.size(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(local.at(j));

            if (j == 2)
            {
                ui->tableWidget->setItem(i, 3, item);
            }
            else
            {
                ui->tableWidget->setItem(i, j, item);
            }
        }


        qDebug() << "Adding download button ...";
        QWidget *widget     = new QWidget;
        QPushButton *button = new QPushButton;
        button->setText(status);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        if (status == "Installed")
        {
            button->setEnabled(false);
        }

        QHBoxLayout *bLayout = new QHBoxLayout(widget);
        bLayout->addWidget(button);
        bLayout->setAlignment(Qt::AlignCenter);
        bLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(bLayout);
        ui->tableWidget->setCellWidget(i, 2, widget);

        QUrl url =
            "https://github.com/PhilInTheGaps/GM-Companion/raw/master/docs/addons/"
            + addonName + "_" + addonVersion + ".zip";

        connect(button, &QPushButton::clicked, this, [ = ]() {
            downloadAddon(url);
        });
    }
}

// For testing purposes, reading addon information from a local file instead of
// the web
void AddonManager::localTest()
{
    QFile f("/home/phil/drives/git/gm-companion/gm-companion/Addons/versions.txt");

    f.open(QIODevice::ReadOnly);

    downloadedData = f.readAll(); // reply->readAll
    f.close();

    setUpTable();
}

// Returns if an addon is already installed or not or if an update is available
QString AddonManager::getStatus(QString addon, QString version)
{
    qDebug().noquote() << "Checking status of addon:" << addon;

    QString status;

    // Is addon installed?
    if (QDir(QDir::homePath() + "/.gm-companion/addons/" + addon).exists())
    {
        status = "Installed";

        QFile f(
            QDir::homePath() + "/.gm-companion/addons/" + addon + "/version.txt");
        f.open(QIODevice::ReadOnly);
        QString localVersion = f.readAll();
        f.close();

        if (version.replace(".",
                            "").toInt() > localVersion.replace(".", "").toInt())
        {
            status = "Update";
        }
    }
    else
    {
        status = "Download";
    }

    return status;
}

// Open download link in browser, I haven't found a nice way to automate
// downloading and extracting addons
void AddonManager::downloadAddon(QUrl url)
{
    QDesktopServices::openUrl(url);
}

void AddonManager::downloadFile(QUrl url)
{
    QNetworkRequest request(url);

    networkManager.get(request);
}

// Downloads information about available addons and sets up ui accordingly
void AddonManager::fileDownloaded(QNetworkReply *reply)
{
    downloadedData = reply->readAll();

    setUpTable();

    reply->deleteLater();
}

void AddonManager::on_pushButton_documentation_clicked()
{
    QDesktopServices::openUrl(QUrl(
                                  "https://github.com/PhilInTheGaps/GM-Companion/wiki/Addons"));
}
