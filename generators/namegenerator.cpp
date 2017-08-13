#include "namegenerator.h"
#include "ui_namegenerator.h"
#include "functions.h"
#include "managers/settingsmanager.h"

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

    ui->textEdit_names->setFontPointSize(ui->spinBox_pointSize->value());

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

                    QHBoxLayout *hbox = new QHBoxLayout;
                    subFrame->setLayout(hbox);

                    QLabel *subfolderLabel = new QLabel(subfolder);
                    hbox->addWidget(subfolderLabel);

                    QFrame *buttonFrame = new QFrame;
                    QHBoxLayout *buttonLayout = new QHBoxLayout;
                    buttonFrame->setLayout(buttonLayout);
                    buttonFrame->setMaximumWidth(200);
                    hbox->addWidget(buttonFrame);

                    // Male names
                    if (QFile(folder+"/"+subfolder+"/male.txt").exists())
                    {
                        QPushButton *maleButton = new QPushButton;
                        maleButton->setMaximumWidth(200);
                        maleButton->setText(tr("Male"));
                        buttonLayout->addWidget(maleButton);

                        QString maleFile = folder+"/"+subfolder+"/male.txt";

                        connect(maleButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
                        signalMapper->setMapping(maleButton, maleFile);
                    }

                    // Female names
                    if (QFile(folder+"/"+subfolder+"/female.txt").exists())
                    {
                        QPushButton *femaleButton = new QPushButton;
                        femaleButton->setMaximumWidth(200);
                        femaleButton->setText(tr("Female"));
                        buttonLayout->addWidget(femaleButton);

                        QString femaleFile = folder+"/"+subfolder+"/female.txt";

                        connect(femaleButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
                        signalMapper->setMapping(femaleButton, femaleFile);
                    }
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

                    QHBoxLayout *hbox = new QHBoxLayout;
                    subFrame->setLayout(hbox);

                    QLabel *subfolderLabel = new QLabel(subfolder);
                    hbox->addWidget(subfolderLabel);

                    QFrame *buttonFrame = new QFrame;
                    QHBoxLayout *buttonLayout = new QHBoxLayout;
                    buttonFrame->setLayout(buttonLayout);
                    buttonFrame->setMaximumWidth(200);
                    hbox->addWidget(buttonFrame);

                    if (QFile(folder+"/"+subfolder+"/male.txt").exists())
                    {
                        QPushButton *maleButton = new QPushButton;
                        maleButton->setMaximumWidth(200);
                        maleButton->setText(tr("Male"));
                        buttonLayout->addWidget(maleButton);

                        QString maleFile = folder+"/"+subfolder+"/male.txt";

                        connect(maleButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
                        signalMapper->setMapping(maleButton, maleFile);
                    }

                    if (QFile(folder+"/"+subfolder+"/female.txt").exists())
                    {
                        QPushButton *femaleButton = new QPushButton;
                        femaleButton->setMaximumWidth(200);
                        femaleButton->setText(tr("Female"));
                        buttonLayout->addWidget(femaleButton);

                        QString femaleFile = folder+"/"+subfolder+"/female.txt";

                        connect(femaleButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
                        signalMapper->setMapping(femaleButton, femaleFile);
                    }
                }
            }
            QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
            frameLayout->addSpacerItem(spacer);
        }
    }

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(on_generateNames(QString)));
}

void NameGenerator::on_generateNames(QString file)
{
    ui->textEdit_names->clear();
    int index = file.lastIndexOf("/");
    QString path = file.left(index);

    QString surnamesPath = path+"/surname.txt";

    QFile nameFile(file);
    nameFile.open(QIODevice::ReadOnly);
    QString namesAsString = QString::fromUtf8(nameFile.readAll());
    QStringList names = namesAsString.split(",");
    nameFile.close();

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
        QString name = names.at(rand() % names.size());

        QString displayName = name;

        if (QFile(surnamesPath).exists()){
            QString surname = surnames.at(rand() % surnames.size());
            displayName.append(" "+surname);
        }

        ui->textEdit_names->append(displayName);
    }
}

void NameGenerator::on_spinBox_pointSize_valueChanged(int arg1)
{
    ui->textEdit_names->setFontPointSize(arg1);
}
