#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

void MainWindow::generateNamesTab(){
    QStringList folderList = getFolders(QDir::currentPath()+"/names");
    for (QString folder : folderList){
        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            QFrame *frame = new QFrame;
            scrollArea->setWidget(frame);
            QVBoxLayout *frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            ui->nameTabWidget->addTab(scrollArea, folder);

            folder = QDir::currentPath()+"/names/"+folder;

            QTabWidget *tabWidget = new QTabWidget;
            frameLayout->addWidget(tabWidget);


            QStringList subfolderList = getFolders(folder);

            for (QString subfolder : subfolderList){
                if (!subfolder.contains(".")){
                    QFrame *subFrame = new QFrame;
                    //tabWidget->addTab(subFrame, subfolder);
                    frameLayout->addWidget(subFrame);

                    QHBoxLayout *hbox = new QHBoxLayout;
                    subFrame->setLayout(hbox);

                    QLabel *subfolderLabel = new QLabel(subfolder);
                    hbox->addWidget(subfolderLabel);

                    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
                    hbox->addSpacerItem(spacer);

                    QFrame *buttonFrame = new QFrame;
                    QHBoxLayout *buttonLayout = new QHBoxLayout;
                    buttonFrame->setLayout(buttonLayout);
                    buttonFrame->setMaximumWidth(200);
                    hbox->addWidget(buttonFrame);

                    QPushButton *maleButton = new QPushButton;
                    maleButton->setText("Male");
                    buttonLayout->addWidget(maleButton);

                    QPushButton *femaleButton = new QPushButton;
                    femaleButton->setText("Female");
                    buttonLayout->addWidget(femaleButton);

                    QStringList files = getFiles(folder+"/"+subfolder);

                    QString maleFile = folder+"/"+subfolder+"/male.txt";
                    QString femaleFile = folder+"/"+subfolder+"/female.txt";

                    connect(maleButton, SIGNAL(clicked()), signalMapperNames, SLOT(map()));
                    signalMapperNames->setMapping(maleButton, maleFile);

                    connect(femaleButton, SIGNAL(clicked()), signalMapperNames, SLOT(map()));
                    signalMapperNames->setMapping(femaleButton, femaleFile);
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

    QFile surnamesFile(surnamesPath);
    surnamesFile.open(QIODevice::ReadOnly);
    QString surnamesAsString = QString::fromUtf8(surnamesFile.readAll());
    ui->textEdit->append(surnamesAsString);
    QStringList surnames = surnamesAsString.split(",");
    surnamesFile.close();

    for (int i = 0; i<20; i++){
        QString name = names.at(rand() % names.size());
        QString surname = surnames.at(rand() % surnames.size());

        if (name.at(0)==" "){
            name = name.mid(1);
        }

        ui->nameTextEdit->append(name+" "+surname);
    }
}
