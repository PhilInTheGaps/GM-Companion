#include "namegenerator.h"
#include "ui_namegenerator.h"
#include "gm-companion/functions.h"
#include "gm-companion/managers/settingsmanager.h"

#include <QDebug>
#include <QScrollArea>
#include <QDir>
#include <QPushButton>

NameGenerator::NameGenerator(QWidget *parent) : QWidget(parent), ui(new Ui::NameGenerator)
{
    qDebug().noquote() << "Loading NameGenerator ...";

    ui->setupUi(this);

    signalMapper = new QSignalMapper;
    settingsManager = new SettingsManager;

    ui->textEdit_male->setFontPointSize(ui->spinBox_pointSize->value());
    ui->textEdit_female->setFontPointSize(ui->spinBox_pointSize->value());

    generateNamesTab();
}

NameGenerator::~NameGenerator()
{
    delete ui;
}

void NameGenerator::generateNamesTab()
{
    // Normal Names
    QStringList folderList = getFolders(QDir::homePath()+"/.gm-companion/names");
    for (QString folder : folderList)
    {
        if (!folder.contains("."))
        {
            // ScrollArea for the name buttons
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            QFrame *frame = new QFrame;
            scrollArea->setWidget(frame);
            QVBoxLayout *frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            ui->tabWidget_categories->addTab(scrollArea, folder);

            folder = QDir::homePath()+"/.gm-companion/names/"+folder;

            QStringList subfolderList = getFolders(folder);

            for (QString subfolder : subfolderList)
            {
                if (!subfolder.contains("."))
                {
                    QFrame *subFrame = new QFrame;
                    frameLayout->addWidget(subFrame);

                    QPushButton *button = new QPushButton;
                    button->setMinimumWidth(200);
                    button->setText(subfolder);
                    frameLayout->addWidget(button);

                    QString path = folder+"/"+subfolder;

                    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
                    signalMapper->setMapping(button, path);

                }
            }
            QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
            frameLayout->addSpacerItem(spacer);
        }
    }

    // Addon Names
    folderList = getFolders(QDir::homePath()+"/.gm-companion/addons");
    for (QString folder : folderList)
    {
        if (!folder.contains(".") && QDir(QDir::homePath()+"/.gm-companion/addons/"+folder+"/names").exists() && settingsManager->getIsAddonEnabled(folder))
        {
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            QFrame *frame = new QFrame;
            scrollArea->setWidget(frame);
            QVBoxLayout *frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            ui->tabWidget_categories->addTab(scrollArea, folder);

            folder = QDir::homePath()+"/.gm-companion/addons/"+folder+"/names";

            QStringList subfolderList = getFolders(folder);

            for (QString subfolder : subfolderList)
            {
                if (!subfolder.contains("."))
                {
                    QFrame *subFrame = new QFrame;
                    frameLayout->addWidget(subFrame);

                    QPushButton *button = new QPushButton;
                    button->setMinimumWidth(200);
                    button->setText(subfolder);
                    frameLayout->addWidget(button);

                    QString path = folder+"/"+subfolder;

                    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
                    signalMapper->setMapping(button, path);

                }
            }
            QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
            frameLayout->addSpacerItem(spacer);
        }
    }

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(on_generateNames(QString)));
}

void NameGenerator::on_generateNames(QString path)
{
    ui->textEdit_male->clear();
    ui->textEdit_male->append("Male Names:");

    ui->textEdit_female->clear();
    ui->textEdit_female->append("Female Names:");

    QString malePath = path+"/male.txt";
    QString femalePath = path+"/female.txt";
    QString surnamesPath = path+"/surname.txt";

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

    for (int i = 0; i<ui->spinBox_amount->value(); i++)
    {
        QString maleName;
        QString femaleName;
        QString surname;

        if (QFile(surnamesPath).exists()){
            surname = surnames.at(rand() % surnames.size());
        }

        if (QFile(malePath).exists())
        {
            maleName = maleNames.at(rand() % maleNames.size());
            maleName.append(" "+surname);

            ui->textEdit_male->append(maleName);
        }

        if (QFile(femalePath).exists())
        {
            femaleName = femaleNames.at(rand() % femaleNames.size());
            femaleName.append(" "+surname);

            ui->textEdit_female->append(femaleName);
        }
    }
}

void NameGenerator::on_spinBox_pointSize_valueChanged(int arg1)
{
    ui->textEdit_male->setFontPointSize(arg1);
    ui->textEdit_female->setFontPointSize(arg1);
}
