#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QDebug>

AboutDialog::AboutDialog(QString version, QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    qDebug() << "Opening AboutDialog ...";

    ui->setupUi(this);

    ui->label_version->setText(tr("Version: ") + version);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
