#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

void MainWindow::generateSoundButtons(){
    tabWidgetSound = new QTabWidget;
    ui->pageSound->layout()->addWidget(tabWidgetSound);

    QStringList soundCategories = getFolders(settingsManager->getSetting(Setting::soundPath)); // List of all categories
    for (QString folder : soundCategories){

        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;

            tabWidgetSound->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

            // Generating musicButtons
            QString path = settingsManager->getSetting(Setting::soundPath)+"/"+folder;
            QStringList soundFolders = getFolders(path);

            FlowLayout *flowLayoutSound = new FlowLayout;
            frame->setLayout(flowLayoutSound);

            for (QString s : soundFolders){
                if (!s.contains(".")){
                    QPushButton *b = new QPushButton();
                    b->setStyleSheet("min-width: 155; min-height: 155; padding: 1px; border-radius: 2px");
                    connect(b, SIGNAL(clicked()), signalMapperSound, SLOT(map()));
                    signalMapperSound->setMapping(b, settingsManager->getSetting(Setting::soundPath)+"/"+folder+"/"+s);

                    //Setting Icon
                    if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png").exists()){
                        QIcon *icon = new QIcon(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg").exists()){
                        QIcon *icon = new QIcon(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else{
                        b->setText(cleanText(s));
                    }

                    b->setFixedSize(155, 155);
                    b->setToolTip(cleanText(s));

                    flowLayoutSound->addWidget(b);
                }
            }
        }
    }
}
