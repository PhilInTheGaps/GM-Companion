#include "namegenerator.h"
#include "ui_namegenerator.h"
#include "gm-companion/functions.h"
#include "gm-companion/settings/settingsmanager.h"

#include <QDebug>
#include <QScrollArea>
#include <QDir>
#include <QPushButton>
#include <QTextEdit>

NameGenerator::NameGenerator(QWidget *parent) : QWidget(parent), ui(new Ui::NameGenerator)
{
    qDebug().noquote() << "Loading NameGenerator ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

    ui->textEdit_male->setFontPointSize(ui->spinBox_pointSize->value());
    ui->textEdit_female->setFontPointSize(ui->spinBox_pointSize->value());

    generateNamesTab();
}

NameGenerator::~NameGenerator()
{
    delete ui;
}

// Create Tabs for every type of names
void NameGenerator::generateNamesTab()
{
    // Names in resources
    generateNameButtons(":/names/Generic", "Generic", true);

    // Custom names in names folder
    for (QString folder : getFolders(QDir::homePath() + "/.gm-companion/names"))
    {
        if (!folder.contains(".")) generateNameButtons(QDir::homePath() + "/.gm-companion/names/" + folder, folder);
    }

    // Addon names
    for (QString folder : getFolders(QDir::homePath() + "/.gm-companion/addons"))
    {
        if (!folder.contains(".") && QDir(QDir::homePath() + "/.gm-companion/addons/" + folder + "/names").exists() && settingsManager->getIsAddonEnabled(folder))
        {
            generateNameButtons(QDir::homePath() + "/.gm-companion/addons/" + folder + "/names", folder);
        }
    }
}

void NameGenerator::generateNameButtons(QString path, QString folder, bool resource)
{
    QScrollArea *scrollArea = new QScrollArea;

    scrollArea->setWidget(new QWidget);
    scrollArea->widget()->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    if (resource)
    {
        QStringList types;

        for (QString r : QDir(path).entryList())
        {
            QString type = r.left(r.indexOf("/"));

            if (!types.contains(type))
            {
                types.append(type);
                addButton(path + "/" + type, type, layout);
            }
        }
    }
    else
    {
        for (QString subfolder : getFolders(path))
        {
            if (!subfolder.contains(".")) addButton(path + "/" + subfolder, subfolder, layout);
        }
    }

    scrollArea->widget()->setLayout(layout);
    scrollArea->widget()->adjustSize();
    ui->tabWidget_categories->addTab(scrollArea, folder);
    ui->tabWidget_categories->adjustSize();
}

void NameGenerator::addButton(QString path, QString subFolder, QVBoxLayout *layout)
{
    QPushButton *button = new QPushButton;

    button->setText(subFolder);
    button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, this, [ = ]() { generateNames(path); });
}

void NameGenerator::generateNames(QString path)
{
    ui->textEdit_male->clear();
    ui->textEdit_female->clear();

    QString malePath     = path + "/male.txt";
    QString femalePath   = path + "/female.txt";
    QString surnamesPath = path + "/surname.txt";

    // Male Names
    QStringList maleNames;

    if (QFile(malePath).exists())
    {
        QFile maleFile(malePath);
        maleFile.open(QIODevice::ReadOnly);
        QString maleNamesAsString = QString::fromUtf8(maleFile.readAll());
        maleNames = maleNamesAsString.split(",");
        maleFile.close();
    }

    // Female Names
    QStringList femaleNames;

    if (QFile(femalePath).exists())
    {
        QFile femaleFile(femalePath);
        femaleFile.open(QIODevice::ReadOnly);
        QString femaleNamesAsString = QString::fromUtf8(femaleFile.readAll());
        femaleNames = femaleNamesAsString.split(",");
        femaleFile.close();
    }

    // Surnames
    QStringList surnames;

    if (QFile(surnamesPath).exists())
    {
        QFile surnamesFile(surnamesPath);
        surnamesFile.open(QIODevice::ReadOnly);
        QString surnamesAsString = QString::fromUtf8(surnamesFile.readAll());
        surnames = surnamesAsString.split(",");
        surnamesFile.close();
    }

    for (int i = 0; i < ui->spinBox_amount->value(); i++)
    {
        QString maleName;
        QString femaleName;
        QString surname;

        if (QFile(surnamesPath).exists()) {
            surname = surnames.at(rand() % surnames.size());
        }

        if (QFile(malePath).exists())
        {
            maleName = maleNames.at(rand() % maleNames.size());
            maleName.append(" " + surname);

            ui->textEdit_male->append(maleName);
        }

        if (QFile(femalePath).exists())
        {
            femaleName = femaleNames.at(rand() % femaleNames.size());
            femaleName.append(" " + surname);

            ui->textEdit_female->append(femaleName);
        }
    }
}

void NameGenerator::on_spinBox_pointSize_valueChanged(int arg1)
{
    ui->textEdit_male->setFontPointSize(arg1);
    ui->textEdit_female->setFontPointSize(arg1);

    ui->textEdit_male->setText(ui->textEdit_male->toPlainText());
    ui->textEdit_female->setText(ui->textEdit_female->toPlainText());
}
