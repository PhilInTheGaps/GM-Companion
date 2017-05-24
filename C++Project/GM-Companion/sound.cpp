#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <QToolButton>

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

            if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png); background-attachment: fixed;}");
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg); background-attachment: fixed;}");
            }

            // Generating soundButtons
            QString path = settingsManager->getSetting(Setting::soundPath)+"/"+folder;
            QStringList soundFolders = getFolders(path);

            FlowLayout *flowLayoutSound = new FlowLayout;
            frame->setLayout(flowLayoutSound);

            for (QString s : soundFolders){
                if (!s.contains(".")){
                    QToolButton* b = new QToolButton;
                    connect(b, SIGNAL(clicked()), signalMapperSound, SLOT(map()));
                    signalMapperSound->setMapping(b, settingsManager->getSetting(Setting::soundPath)+"/"+folder+"/"+s);

                    if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png").exists()){
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px; "
                                         "background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png); "
                                         "background-repeat: no-repeat; "
                                         "background-position: center center;}"
                                         "QToolButton QWidget{"
                                         "color: white} ");
                    }
                    else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg").exists()){
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px; "
                                         "background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg); "
                                         "background-repeat: no-repeat; "
                                         "background-position: center center;}"
                                         "QToolButton QWidget{"
                                         "color: white} ");
                    }
                    else if (QFile("resources/button.png").exists()){
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px; "
                                         "background-image: url("+QApplication::applicationDirPath()+"/resources/button.png); "
                                         "background-repeat: no-repeat; "
                                         "background-position: center center;}"
                                         "QToolButton QWidget{"
                                         "color: white} ");
                    }
                    else{
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px;");
                    }

                    QFont font;
                    font.setPixelSize(15);
                    b->setFont(font);

                    QString title = cleanText(s);

                    if (title.length()>20){
                        int i = title.indexOf(" ", 14);
                        title.remove(i, 1);
                        title.insert(i, "\n");
                    }

                    b->setText(title);

                    b->setFixedSize(152, 152);
                    b->setToolTip(cleanText(s));

                    flowLayoutSound->addWidget(b);
                }
            }
        }
    }
}
