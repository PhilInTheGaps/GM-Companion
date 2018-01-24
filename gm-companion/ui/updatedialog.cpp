#include "updatedialog.h"
#include "ui_updatedialog.h"

#include <QDesktopServices>
#include <QUrl>

UpdateDialog::UpdateDialog(QString title, QString versionString,
                           QWidget *parent) : QWidget(parent), ui(
        new Ui::UpdateDialog)
{
    ui->setupUi(this);

    ui->label_title->setText(title);
    ui->label_versionNumber->setText(versionString);
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::on_buttonBox_accepted()
{
    QDesktopServices::openUrl(QUrl(
                                  "https://github.com/PhilInTheGaps/GM-Companion/releases/tag/"
                                  +
                                  ui->label_versionNumber->text()));
}

void UpdateDialog::on_buttonBox_rejected()
{
    close();
}
