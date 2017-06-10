#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

void MainWindow::generateNamesTab(){
    QStringList folderList = getFolders(QDir::homePath()+"/.gm-companion/names");
    for (QString folder : folderList){
        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            QFrame *frame = new QFrame;
            scrollArea->setWidget(frame);
            QVBoxLayout *frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            ui->nameTabWidget->addTab(scrollArea, folder);

            folder = QDir::homePath()+"/.gm-companion/names/"+folder;

            QStringList subfolderList = getFolders(folder);

            for (QString subfolder : subfolderList){
                if (!subfolder.contains(".")){
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

                    if (QFile(folder+"/"+subfolder+"/male.txt").exists()){
                        QPushButton *maleButton = new QPushButton;
                        maleButton->setMaximumWidth(200);
                        maleButton->setText(tr("Male"));
                        buttonLayout->addWidget(maleButton);

                        QString maleFile = folder+"/"+subfolder+"/male.txt";

                        connect(maleButton, SIGNAL(clicked()), signalMapperNames, SLOT(map()));
                        signalMapperNames->setMapping(maleButton, maleFile);
                    }

                    if (QFile(folder+"/"+subfolder+"/female.txt").exists()){
                        QPushButton *femaleButton = new QPushButton;
                        femaleButton->setMaximumWidth(200);
                        femaleButton->setText(tr("Female"));
                        buttonLayout->addWidget(femaleButton);

                        QString femaleFile = folder+"/"+subfolder+"/female.txt";

                        connect(femaleButton, SIGNAL(clicked()), signalMapperNames, SLOT(map()));
                        signalMapperNames->setMapping(femaleButton, femaleFile);
                    }
                }
            }
            QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
            frameLayout->addSpacerItem(spacer);
        }
    }
    connect(signalMapperNames, SIGNAL(mapped(QString)), this, SLOT(on_generateNames(QString)));
}

void MainWindow::on_generateNames(QString file){
    ui->nameTextEdit->clear();
    int index = file.lastIndexOf("/");
    QString path = file.left(index);

    QString surnamesPath = path+"/surname.txt";

    QFile nameFile(file);
    nameFile.open(QIODevice::ReadOnly);
    QString namesAsString = QString::fromUtf8(nameFile.readAll());
    QStringList names = namesAsString.split(",");
    nameFile.close();

    QStringList surnames;

    if (QFile(surnamesPath).exists()){
        QFile surnamesFile(surnamesPath);
        surnamesFile.open(QIODevice::ReadOnly);
        QString surnamesAsString = QString::fromUtf8(surnamesFile.readAll());
        surnames = surnamesAsString.split(",");
        surnamesFile.close();
    }

    for (int i = 0; i<ui->namesAmountSpinBox->value(); i++){
        QString name = names.at(rand() % names.size());

        if (name.at(0)==" "){
            name = name.mid(1);
        }

        QString displayName = name;

        if (QFile(surnamesPath).exists()){
            QString surname = surnames.at(rand() % surnames.size());
            displayName.append(" "+surname);
        }

        ui->nameTextEdit->append(displayName);
    }
}
