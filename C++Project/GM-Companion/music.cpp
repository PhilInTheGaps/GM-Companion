#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

void MainWindow::generateMusicButtons(){
    tabWidgetMusic = new QTabWidget;
    ui->pageMusic->layout()->addWidget(tabWidgetMusic);

    QStringList musicCategories = getFolders(settingsManager->getSetting(Setting::musicPath)); // List of all categories
    for (QString folder : musicCategories){

        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;

            tabWidgetMusic->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

            if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png); background-attachment: fixed;}");
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg); background-attachment: fixed;}");
            }

            // Generating musicButtons
            QString path = settingsManager->getSetting(Setting::musicPath)+"/"+folder;
            QStringList musicFolders = getFolders(path);

            FlowLayout *flowLayoutMusic = new FlowLayout;
            frame->setLayout(flowLayoutMusic);

            for (QString s : musicFolders){
                if (!s.contains(".")){
                    QPushButton *b = new QPushButton(); //cleanText(s)
                    b->setStyleSheet("min-width: 155; min-height: 155; padding: 1px; border-radius: 2px");
                    connect(b, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
                    signalMapperMusic->setMapping(b, settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/"+s);

                    //Setting Icon
                    if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png").exists()){
                        QIcon *icon = new QIcon(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
                        QIcon *icon = new QIcon(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else{
                        QFont font;
                        font.setPixelSize(25);
                        b->setFont(font);
                        b->setText(cleanText(s));
                    }

                    b->setFixedSize(155, 155);
                    b->setToolTip(cleanText(s));

                    flowLayoutMusic->addWidget(b);
                }
            }
        }
    }
}
